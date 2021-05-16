#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "SceneDungeon.h"
#include "SceneBattle.h"
#include "Player.h"
#include "Enemy1.h"
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


Enemy1::Enemy1() : Entity(EntityType::Enemy1)
{
	name.Create("Enemy1");
	position.x = 643;
	position.y = 343;

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
Enemy1::~Enemy1() {}

bool Enemy1::Awake()
{
	LOG("Loading Enemy1: Beach Girl");

	bool ret = true;
	return ret;
}

bool Enemy1::Start()
{
	if (this->active == true)
	{
		texEnemy1 = app->tex->Load("Assets/Textures/enemy1_character.png");
		Enemy1InitialPosition();
		currentAnimation = &idlAnim;
	}
	return true;
}

bool Enemy1::Update(float dt)
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
	else if (app->sceneDungeon->active == true)
	{
		currentAnimation = &idlAnim;
		position.x = 49;
		position.y = 240;
	}

	if (app->sceneBattle->battleOn)
	{
		currentAnimation = &idlAnim;
	}
	


	currentAnimation->Update();
	return true;
}

bool Enemy1::PostUpdate()
{
	if ((this->active == true) && (!app->scene->paused) && (!app->sceneBattle->enemy1Dead))
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texEnemy1, position.x, position.y, &rect);
	}
	return true;
}

bool Enemy1::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texEnemy1);
	return true;
}

void Enemy1::Enemy1InitialPosition()
{
	if (app->scene->active == true)
	{
		position.x = 643;
		position.y = 343;
	}
	if (app->sceneBattle->battleOn && app->scene->active == false)
	{
		position.x = 300;
		position.y = 100;
	}

}