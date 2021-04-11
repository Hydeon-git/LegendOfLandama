#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "FlyingEnemy.h"
#include "Pathfinding.h"
#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"


#define COLLIDER_GREEN 265
#define COLLIDER_RED 266
#define COLLIDER_BLUE 267
#define COLLIDER_YELLOW 268

FlyingEnemy::FlyingEnemy() : Entity(EntityType::FLYING_ENEMY)
{
	name.Create("flyingenemy");
	position.x = 4000;
	position.y = 500;

	//idlanim
	idlAnim.PushBack({ 0, 0, 64, 52 });
	idlAnim.PushBack({ 0, 52, 64, 52 });
	idlAnim.speed = 0.03f;

	//deathAnim
	deathAnim.PushBack({ 0, 107, 64, 52 });
	deathAnim.loop = false;
	deathAnim.speed = 0.02f;
}

// Destructor
FlyingEnemy::~FlyingEnemy()
{

}

bool FlyingEnemy::Awake()
{
	LOG("Loading Flying Enemy");
	bool ret = true;

	return ret;
}

bool FlyingEnemy::Start()
{
	if (app->scene->flyingEnemy!=nullptr)
	{
		dead = false;
		texFlyingEnemy = app->tex->Load("Assets/Textures/flying_enemy_texture.png");
		flyingEnemyDeathFx = app->audio->LoadFx("Assets/Audio/Fx/enemy_death.wav");

		currentAnimation = &idlAnim;
	}
	return true;
}

bool FlyingEnemy::Update(float dt)
{

	if (!dead && !app->scene->player->spiked && !app->scene->player->godModeEnabled)
	{
		currentAnimation = &idlAnim;
		if ((position.DistanceTo(app->scene->player->position) < 500))
		{
			currentAnimation = &idlAnim;

			posOrigin = app->map->WorldToMap(position.x, position.y);
			posDestination = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

			app->pathfinding->CreatePath(posOrigin, posDestination);
			const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

			if (path->At(1) != NULL)
			{
				if (path->At(1)->x < posOrigin.x && !ThereIsLeftWall()) position.x -= speed;

				else if (path->At(1)->x > posOrigin.x && !ThereIsRightWall()) position.x += speed;

				if (path->At(1)->y < posOrigin.y && !ThereIsTopWall()) position.y -= speed;

				else if (path->At(1)->y > posOrigin.y && !ThereIsGround()) position.y += speed;
			}

			if (posOrigin == posDestination)
			{
				if (position.x > app->scene->player->position.x) position.x -= speed;

				else if (position.x < app->scene->player->position.x) position.x += speed;

				if (position.y > app->scene->player->position.y) position.y -= speed;

				else if (position.y < app->scene->player->position.y) position.y += speed;
			}
		}
	}

	if (deathAnim.HasFinished())
	{
		active = false;
		deathAnim.Reset();
	}
	currentAnimation->Update();
	return true;
}

bool FlyingEnemy::PostUpdate()
{
	if (this->active == true)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texFlyingEnemy, position.x, position.y, &rect);

		const DynArray<iPoint>* pathDraw = app->pathfinding->GetLastPath();
		if (app->map->colliders)
		{
			for (uint i = 0; i < pathDraw->Count(); ++i)
			{
				iPoint nextPos = app->map->MapToWorld(pathDraw->At(i)->x, pathDraw->At(i)->y);
				SDL_Rect rectPath = { nextPos.x, nextPos.y, 64, 64 };
				app->render->DrawRectangle(rectPath, 0, 0, 255, 100);
			}
			app->pathfinding->lastPath.Clear();
		}
	}
	return true;
}

bool FlyingEnemy::ThereIsGround()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 31; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + i + 16, position.y + 50);
				groundId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (groundId == COLLIDER_RED) valid = true;
			}

		}
		layer = layer->next;
	}
	return valid;

}

bool FlyingEnemy::ThereIsLeftWall()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 31; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x, position.y + i + 7);
				groundId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (groundId == COLLIDER_RED) valid = true;
			}
		}
		layer = layer->next;
	}
	return valid;
}

bool FlyingEnemy::ThereIsRightWall()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 31; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + 62, position.y + i + 7);
				groundId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (groundId == COLLIDER_RED) valid = true;
			}
		}
		layer = layer->next;
	}
	return valid;
}

bool FlyingEnemy::ThereIsTopWall()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int groundId;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 31; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + i + 16, position.y);
				groundId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (groundId == COLLIDER_RED) valid = true;
			}
		}
		layer = layer->next;
	}
	return valid;
}

bool FlyingEnemy::IsDead()
{
	dead = true;
	currentAnimation = &deathAnim;
	app->audio->PlayFx(flyingEnemyDeathFx, 0);
	return true;
}

bool FlyingEnemy::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texFlyingEnemy);
	//app->entityManager->DestroyEntity(this);

	return true;
}


bool FlyingEnemy::LoadState(pugi::xml_node& node)
{
	bool ret = true;
	position.x = node.child("positionFlyingEnemy").attribute("x").as_int();
	position.y = node.child("positionFlyingEnemy").attribute("y").as_int();
	return ret;
}

bool FlyingEnemy::SaveState(pugi::xml_node& node) const
{
	bool ret = true;
	pugi::xml_node pnode = node.append_child("positionFlyingEnemy");
	pnode.append_attribute("x") = position.x;
	pnode.append_attribute("y") = position.y;
	return ret;
}

void FlyingEnemy::FlyingEnemyInitialPosition()
{
	position.x = 4000;
	position.y = 500;
}
