#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy3.h"
#include "ModuleParticles.h"
#include "Pathfinding.h"
#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"
#include "SceneBattle.h"

#define COLLIDER_GREEN 265
#define COLLIDER_RED 266
#define COLLIDER_BLUE 267
#define COLLIDER_YELLOW 268


Enemy3::Enemy3() : Entity(EntityType::Enemy3)
{
	name.Create("Enemy3");
	position.x = 660;
	position.y = 336;

	//idlanim
	idlAnim.PushBack({ 127, 0,12, 18 });
	idlAnim.PushBack({ 143, 0, 12,18 });
	idlAnim.speed = 0.02f;

	//move right
	rightAnim.PushBack({ 0, 0, 12, 18 });
	rightAnim.PushBack({ 16, 0, 12, 18 });
	rightAnim.PushBack({ 32, 0, 12, 18 });
	rightAnim.PushBack({ 48, 0, 12, 18 });
	rightAnim.PushBack({ 64, 0, 12,	18 });
	rightAnim.PushBack({ 79, 0, 13,18 });
	rightAnim.PushBack({ 96, 0, 12,18 });
	rightAnim.PushBack({ 112, 0, 12,18 });

	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 127, 0,12, 18 });
	leftAnim.PushBack({ 143, 0, 12,18 });
	leftAnim.PushBack({ 159,0, 13, 18 });
	leftAnim.PushBack({ 175,0, 12, 18 });
	leftAnim.PushBack({ 191,0, 12, 18 });
	leftAnim.PushBack({ 207,0, 12, 18 });
	leftAnim.PushBack({ 223,0, 12, 18 });
	leftAnim.PushBack({ 239,0, 12, 18 });

	leftAnim.speed = 0.1f;

	currentAnimation = &idlAnim;
}

// Destructor
Enemy3::~Enemy3() {}

bool Enemy3::Awake()
{
	LOG("Loading Enemy3: Beach Girl");

	bool ret = true;
	return ret;
}

bool Enemy3::Start()
{
	if (this->active == true)
	{
		texEnemy3 = app->tex->Load("Assets/Textures/Enemy3_character.png");
		Enemy3InitialPosition();
		currentAnimation = &idlAnim;
	}
	return true;
}

bool Enemy3::Update(float dt)
{
	currentAnimation = &leftAnim;

	if (app->scene->active == true)
	{
		if (position.x <= 502) app->scene->stop = true;

		if (!app->scene->stop)
		{
			if (app->scene->player->position.x >= 460) app->scene->enemyMoving = true;
			if (app->scene->enemyMoving) position.x -= 2.0f;

		}
		if (app->scene->stop)
		{
			currentAnimation = &idlAnim;
		}
	}
	if (app->sceneBattle->battleOn)
	{
		currentAnimation = &idlAnim;
	}

	currentAnimation->Update();
	return true;
}

bool Enemy3::PostUpdate()
{
	if ((this->active == true) && (!app->scene->paused))
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texEnemy3, position.x, position.y, &rect);
	}
	return true;
}

bool Enemy3::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texEnemy3);
	return true;
}

void Enemy3::Enemy3InitialPosition()
{
	if (app->scene->active == true)
	{
		position.x = 660;
		position.y = 336;
	}
	if (app->sceneBattle->battleOn && app->scene->active == false)
	{
		position.x = 315;
		position.y = 120;
	}
}