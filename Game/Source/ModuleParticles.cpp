#include "ModuleParticles.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"
#include "EntityManager.h"
#include "Scene.h"

#define COLLIDER_RED 266

Particle::Particle()/* : Entity(EntityType::PARTICLE)*/
{
	position.SetToZero();
	speed.SetToZero();
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), speed(p.speed), frameCount(p.frameCount), lifeTime(p.lifeTime)/*, Entity(EntityType::PARTICLE)*/
{

}

Particle::~Particle()
{

}

bool Particle::Update()
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the delay has been reached
	if (!isAlive && frameCount >= 0) isAlive = true;

	if (isAlive)
	{
		anim.Update();

		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifeTime > 0)
		{
			if (frameCount >= lifeTime) ret = false;
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (anim.HasFinished()) ret = false;

		// Update the position in the screen
		position.x += speed.x;
		position.y += speed.y;

	}
	if (ThereIsEnemy())
	{
		if (!app->scene->enemy->dead)
		{
			app->scene->enemy->IsDead();
			SetToDelete();
		}

	}
	if (ThereIsFlyingEnemy())
	{
		if (!app->scene->flyingEnemy->dead)
		{
			app->scene->flyingEnemy->IsDead();
			SetToDelete();
		}
	}
	if (ThereIsLeftWall())
	{
		SetToDelete();
	}
	if (ThereIsRightWall())
	{
		SetToDelete();
	}
	return ret;
}

void Particle::SetToDelete()
{

	pendingToDelete = true;

}

bool Particle::ThereIsEnemy()
{
	bool valid = false;
	bool positionX = false;
	bool positionY = false;

	for (int i = 0; i < 30; ++i)
	{
		for (int j = 0; j < 22; ++j)
		{
			if (app->scene->enemy->position.x + 16 + i == position.x + j) positionX = true;
		}
	}
	for (int i = 0; i < 62; ++i)
	{
		for (int j = 0; j < 14; ++j)
		{
			if (app->scene->enemy->position.y + 22 + i == position.y + j) positionY = true;
		}
	}

	if (positionX && positionY) valid = true;

	return valid;
}

bool Particle::ThereIsFlyingEnemy()
{
	bool valid = false;
	bool positionX = false;
	bool positionY = false;

	for (int i = 0; i < 60; ++i)
	{
		for (int j = 0; j < 22; ++j)
		{
			if (app->scene->flyingEnemy->position.x + i == position.x + j) positionX = true;
		}
	}
	for (int i = 0; i < 50; ++i)
	{
		for (int j = 0; j < 14; ++j)
		{
			if (app->scene->flyingEnemy->position.y + i == position.y + j) positionY = true;
		}
	}

	if (positionX && positionY) valid = true;

	return valid;
}

bool Particle::ThereIsLeftWall()
{
	bool valid = false;

	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x, position.y);
				groundId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (groundId == COLLIDER_RED) valid = true;
			}
		}
		layer = layer->next;
	}

	return valid;

}

bool Particle::ThereIsRightWall()
{
	bool valid = false;

	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		for (int i = 0; i < 4; ++i)
		{
			tilePosition = app->map->WorldToMap(position.x + 25, position.y);
			groundId = layer->data->Get(tilePosition.x, tilePosition.y);
			if (groundId == COLLIDER_RED) valid = true;
		}
		layer = layer->next;
	}

	return valid;
}





ModuleParticles::ModuleParticles() : Entity(EntityType::PARTICLE)
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	LOG("Loading particles");
	texture = app->tex->Load("Assets/Textures/shot_fireball.png");


	fireBallRight.anim.PushBack({ 0, 0, 22, 14 });
	fireBallRight.speed.x = 5;
	fireBallRight.lifeTime = 70;
	fireBallRight.anim.speed = 0.2f;

	fireBallLeft.anim.PushBack({ 0, 14, 22, 14 });
	fireBallLeft.speed.x = -5;
	fireBallLeft.lifeTime = 70;
	fireBallLeft.anim.speed = 0.2f;

	return true;
}

bool ModuleParticles::PreUpdate()
{
	// Remove all particles scheduled for deletion
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr && particles[i]->pendingToDelete)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if(particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}
	//app->entityManager->DestroyEntity(this);

	return true;
}

bool ModuleParticles::Update(float dt)
{
	for(uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if(particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if(particle->Update() == false)
		{
			particles[i]->SetToDelete();
		}
	}

	return true;
}

bool ModuleParticles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			app->render->DrawTexture(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
		}
	}

	return true;
}

Particle* ModuleParticles::AddParticle(const Particle& particle, int x, int y, uint delay)
{
	Particle* newParticle = nullptr;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			newParticle = new Particle(particle);
			newParticle->frameCount = -(int)delay;
			newParticle->position.x = x;
			newParticle->position.y = y;

			//Adding the particle's collider
			particles[i] = newParticle;
			break;
		}
	}

	return newParticle;
}