#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "ModuleParticles.h"
#include "Pathfinding.h"
#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"


#define COLLIDER_GREEN 265
#define COLLIDER_RED 266
#define COLLIDER_BLUE 267
#define COLLIDER_YELLOW 268


Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("enemy");
	position.x = 1000;
	position.y = 875;

	//idlanim
	idlAnim.PushBack({ 0, 0, 64, 85 });
	idlAnim.PushBack({ 0, 430, 64, 85 });
	idlAnim.speed = 0.04f;


	//move right
	rightAnim.PushBack({ 0, 85, 64, 85 });
	rightAnim.PushBack({ 0, 170, 64, 85 });
	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 0, 255, 64, 85 });
	leftAnim.PushBack({ 0, 340, 64, 85 });
	leftAnim.speed = 0.1f;

	//deathAnim
	deathAnim.PushBack({ 0, 518, 64, 85 });
	//deathAnim.PushBack({ 0, 700, 64, 85 });
	deathAnim.loop = false;
	deathAnim.speed = 0.01f;
	
}

// Destructor
Enemy::~Enemy()
{}

bool Enemy::Awake()
{
	LOG("Loading Enemy");
	bool ret = true;

	return ret;
}

bool Enemy::Start()
{
	if (this->active == true)
	{
		dead = false;
		texEnemy = app->tex->Load("Assets/Textures/enemy_texture.png");
		enemyDeathFx = app->audio->LoadFx("Assets/Audio/Fx/enemy_death.wav");

		currentAnimation = &idlAnim;
	}
	return true;
}

bool Enemy::Update(float dt)
{
	if (!dead && !app->scene->player->spiked && !app->scene->player->godModeEnabled)
	{
		currentAnimation = &idlAnim;

		if ((position.DistanceTo(app->scene->player->position) < 300))
		{
			currentAnimation = &idlAnim;
			posOrigin = app->map->WorldToMap(position.x, position.y);
			posDestination = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
			app->pathfinding->CreatePath(posOrigin, posDestination);
			const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

			if (path->At(1) != NULL)
			{
				if (path->At(1)->x < posOrigin.x && ThereIsGroundLeft())
				{
					position.x -= speed;
					currentAnimation = &leftAnim;
				}
				else if (path->At(1)->x > posOrigin.x && ThereIsGroundRight())
				{
					position.x += speed;
					currentAnimation = &rightAnim;
				}
				if (!ThereIsGroundLeft() && (position.x + 10) % 64 != 0)
				{
					position.x--;
					currentAnimation = &leftAnim;
				}
				else if (!ThereIsGroundRight() && (position.x) % 64 != 0)
				{
					position.x++;
					currentAnimation = &rightAnim;
				}
			}
			if (posOrigin == posDestination)
			{
				if (position.x > app->scene->player->position.x)
				{
					position.x -= speed;
					currentAnimation = &leftAnim;
				}
				else if (position.x < app->scene->player->position.x)
				{
					position.x += speed;
					currentAnimation = &rightAnim;
				}
			}
		}
	}
	if (app->scene->player->spiked && !dead) currentAnimation = &idlAnim;

	if (deathAnim.HasFinished())
	{
		active = false;
		deathAnim.Reset();
	}
	currentAnimation->Update();
	return true;
}

bool Enemy::PostUpdate()
{
	if (this->active == true)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texEnemy, position.x, position.y, &rect);

		const DynArray<iPoint>* pathDraw = app->pathfinding->GetLastPath();
		if (app->map->colliders)
		{
			for (uint i = 0; i < pathDraw->Count(); ++i)
			{
				iPoint nextPos = app->map->MapToWorld(pathDraw->At(i)->x, pathDraw->At(i)->y + 1);
				SDL_Rect rectPath = { nextPos.x, nextPos.y, 64, 64 };
				app->render->DrawRectangle(rectPath, 0, 0, 255, 100);
			}
			app->pathfinding->lastPath.Clear();
		}
	}
	return true;
}

bool Enemy::ThereIsGroundLeft()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 3; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y +85);
				groundId = layer->data->Get(tilePosition.x-1, tilePosition.y);
				if (groundId == COLLIDER_RED) valid = true;
			}

		}
		layer = layer->next;
	}
	return valid;

}

bool Enemy::ThereIsGroundRight()
{
	bool valid = false;
	//if (!godModeEnabled)
	//{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int groundId;
		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 3; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y +85);
					groundId = layer->data->Get(tilePosition.x+1, tilePosition.y);
					if (groundId == COLLIDER_RED) valid = true;
				}

			}
			layer = layer->next;
		}
	//}
	return valid;

}

bool Enemy::IsDead()
{
	dead = true;
	currentAnimation = &deathAnim;
	app->audio->PlayFx(enemyDeathFx, 0);
	return true;
}

bool Enemy::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texEnemy);
	//app->entityManager->DestroyEntity(this);
	return true;
}


bool Enemy::LoadState(pugi::xml_node& node)
{
	bool ret = true;
	position.x = node.child("positionEnemy").attribute("x").as_int();
	position.y = node.child("positionEnemy").attribute("y").as_int();
	return ret;
}

bool Enemy::SaveState(pugi::xml_node& node) const
{
	bool ret = true;
	pugi::xml_node pnode = node.append_child("positionEnemy");
	pnode.append_attribute("x") = position.x;
	pnode.append_attribute("y") = position.y;
	return ret;
}


void Enemy::EnemyInitialPosition()
{
	position.x = 1000;
	position.y = 875;
}