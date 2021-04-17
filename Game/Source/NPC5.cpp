#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "NPC5.h"
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


NPC5::NPC5() : Entity(EntityType::NPC5)
{
	name.Create("NPC5");
	position.x = app->scene->player->lastPositionX;
	position.y = app->scene->player->lastPositionY-5;

	//idlanim
	idlAnim.PushBack({ 0, 0, 15, 23 });
	idlAnim.PushBack({ 16, 0, 15, 23 });
	idlAnim.speed = 0.02f;

	//move right
	rightAnim.PushBack({ 0, 0, 15, 23 });
	rightAnim.PushBack({ 16, 0, 15, 23 });
	rightAnim.PushBack({ 32, 0, 15, 23 });
	rightAnim.PushBack({ 48, 0, 15, 23 });
	rightAnim.PushBack({ 64, 0, 15,23 });
	rightAnim.PushBack({ 80, 0, 15,23 });
	rightAnim.PushBack({ 96, 0, 15,23 });
	rightAnim.PushBack({ 112, 0, 15,23 });
	rightAnim.PushBack({ 128, 0,15, 23 });

	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 145, 0, 15,23 });
	leftAnim.PushBack({ 161,0, 15, 23 });
	leftAnim.PushBack({ 176,0, 15, 23 });
	leftAnim.PushBack({ 193,0, 15, 23 });
	leftAnim.PushBack({ 209,0, 15, 23 });
	leftAnim.PushBack({ 225,0, 15, 23 });
	leftAnim.PushBack({ 241,0, 15, 23 });
	leftAnim.PushBack({ 257,0, 15, 23 });
	leftAnim.PushBack({ 273,0, 15, 23 });

	leftAnim.speed = 0.1f;


	//move up
	upAnim.PushBack({ 288, 0, 15,23 });
	upAnim.PushBack({ 304,0, 15, 23 });
	upAnim.PushBack({ 320,0, 15, 23 });
	upAnim.PushBack({ 336,0, 15, 23 });
	upAnim.PushBack({ 352,0, 15, 23 });
	upAnim.PushBack({ 368,0, 15, 23 });
	upAnim.PushBack({ 384,0, 15, 23 });
	upAnim.PushBack({ 400,0, 15, 23 });
	upAnim.PushBack({ 416,0, 15, 23 });

	upAnim.speed = 0.1f;


	currentAnimation = &idlAnim;
}

// Destructor
NPC5::~NPC5() {}

bool NPC5::Awake()
{
	LOG("Loading NPC5: Ustru Becari");

	bool ret = true;
	return ret;
}

bool NPC5::Start()
{
	if (this->active == true)
	{
		texNPC5 = app->tex->Load("Assets/Textures/NPC5_character.png");

		currentAnimation = &idlAnim;
	}
	return true;
}

bool NPC5::Update(float dt)
{
	
	position.x = app->scene->player->lastPositionX;
	position.y = app->scene->player->lastPositionY-5;


	if (app->scene->player->lastPositionX < app->scene->player->lastPosX[23])
	{
		currentAnimation = &rightAnim;
	}
	else if (app->scene->player->lastPositionX > app->scene->player->lastPosX[23])
	{
		currentAnimation = &leftAnim;
	}
	else if (app->scene->player->lastPositionY > app->scene->player->lastPosY[18])
	{
		currentAnimation = &upAnim;
	}
	else if (app->scene->player->lastPositionY < app->scene->player->lastPosY[18])
	{
		currentAnimation = &rightAnim;
	}
	else
	{
		currentAnimation = &idlAnim;

	}




	currentAnimation->Update();
	return true;
}

bool NPC5::PostUpdate()
{
	if (this->active == true)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texNPC5, position.x, position.y, &rect);
	}
	return true;
}

bool NPC5::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texNPC5);
	return true;
}

void NPC5::NPC5InitialPosition()
{
	position.x = app->scene->player->lastPositionX;
	position.y = app->scene->player->lastPositionY-5;
}