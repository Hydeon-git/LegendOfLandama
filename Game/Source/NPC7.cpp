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
#include "NPC7.h"
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


NPC7::NPC7() : Entity(EntityType::NPC7)
{
	name.Create("NPC7");
	if (app->sceneBattle->battleOn)
	{
		position.x = 105;
		position.y = 122;
	}
	else if(app->sceneBattle->knightRevive)
	{
		position.x = 128;
		position.y = 139;
		app->scene->player->knightTkn = false;
	}
	else {
		//position.x = app->scene->player->lastPositionX;
		//position.y = app->scene->player->lastPositionY - 5;
		position.x = 300;
		position.y = 172;
		//position.x = 200;
		//position.y = 100;

	}
	//idlanim
	idlAnim.PushBack({ 0, 0, 16, 27 });
	idlAnim.PushBack({ 16, 0, 16,27 });
	idlAnim.speed = 0.02f;

	//move right
	rightAnim.PushBack({ 0, 0,  16, 27 });
	rightAnim.PushBack({ 16, 0, 16,27 });
	rightAnim.PushBack({ 32, 0, 16,27 });
	rightAnim.PushBack({ 48, 0, 16,27 });
	rightAnim.PushBack({ 64, 0, 16,27 });
	rightAnim.PushBack({ 80, 0, 16,27 });
	rightAnim.PushBack({ 96, 0, 16,27 });
	rightAnim.PushBack({ 112, 0,16,27 });
	rightAnim.PushBack({ 129, 0,16, 27 });

	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 146,0, 16,27 });
	leftAnim.PushBack({ 162,0, 16, 27 });
	leftAnim.PushBack({ 178,0, 16, 27 });
	leftAnim.PushBack({ 194,0, 16, 27 });
	leftAnim.PushBack({ 210,0, 16, 27 });
	leftAnim.PushBack({ 226,0, 16, 27 });
	leftAnim.PushBack({ 242,0, 16, 27 });
	leftAnim.PushBack({ 258,0, 16, 27 });
	leftAnim.PushBack({ 274,0, 16, 27 });

	leftAnim.speed = 0.1f;


	//move up
	upAnim.PushBack({ 291,0, 16,27 });
	upAnim.PushBack({ 307,0, 16, 27 });
	upAnim.PushBack({ 323,0, 16, 27 });
	upAnim.PushBack({ 339,0, 16, 27 });
	upAnim.PushBack({ 355,0, 16, 27 });
	upAnim.PushBack({ 371,0, 16, 27 });
	upAnim.PushBack({ 387,0, 16, 27 });
	upAnim.PushBack({ 403,0, 16, 27 });
	upAnim.PushBack({ 420,0, 16, 27 });

	upAnim.speed = 0.1f;


	currentAnimation = &idlAnim;
}

// Destructor
NPC7::~NPC7() {}

bool NPC7::Awake()
{
	LOG("Loading NPC7: Knight");

	bool ret = true;
	return ret;
}

bool NPC7::Start()
{
	if (this->active == true)
	{
		texNPC7 = app->tex->Load("Assets/Textures/NPC7_character.png");

		currentAnimation = &idlAnim;
	}
	return true;
}

bool NPC7::Update(float dt)
{

	if (app->scene->player->knightTkn)
	{
		if (!app->sceneBattle->battleOn)
		{

			if (app->scene->player->kDoorTaked2)
			{
				app->scene->player->kposMoved = 0;
				app->scene->player->kDoorTaked2 = false;
			}


			//if ((position.y > app->scene->player->position.y + 24-5 || position.y < app->scene->player->position.y - 24-5))
			//{
			//	position.y = app->scene->player->position.y - 5;
			//	app->scene->player->doorTaked = false;

			//	//app->scene->player->doIt = true;
			//}
			//else


				//movement
			if (app->scene->player->kposMoved < 49)
			{
				if (app->scene->player->klastPositionX != 0) position.x = app->scene->player->klastPositionX;
				if (app->scene->player->klastPositionY != 0) position.y = app->scene->player->klastPositionY - 5;
			}




			if (app->scene->player->kDoorTaked)
			{
				position.x = app->scene->player->klastPositionX2;
				position.y = app->scene->player->klastPositionY2 - 5;
				if (app->scene->player->kposMoved >= 49)
				{
					app->scene->player->kposMoved = 0;
					app->scene->player->kDoorTaked = false;
				}
			}


			//if (!app->scene->player->doorTakedX)
			//{
			//	if (app->scene->player->lastPositionX != 0) position.x = app->scene->player->lastPositionX;
			//	app->scene->player->doorTakedY = false;

			//}
			//if (!app->scene->player->doorTakedY)
			//{
			//	if (app->scene->player->lastPositionY != 0) {
			//		position.y = app->scene->player->lastPositionY - 5;
			//	}
			//	app->scene->player->doorTakedX = false;

			//}




		//if (app->scene->player->doorTakedX)
		//{
		//	position.x = app->scene->player->lastPositionX2;
		//	if (app->scene->player->posMovedX >= 24)
		//	{
		//		app->scene->player->posMovedX = 0;
		//		app->scene->player->doorTakedX = false;
		//	}
		//}
		//if (app->scene->player->doorTakedY)
		//{
		//	position.y = app->scene->player->lastPositionY2 - 5;
		//	if (app->scene->player->posMovedY >= 24)
		//	{
		//		app->scene->player->posMovedY = 0;
		//		app->scene->player->doorTakedY = false;
		//	}
		//}



		//if (!app->scene->player->doorTakedX || !app->scene->player->doorTakedY)
		//rotation
			if (!app->scene->player->kDoorTaked)
			{
				if (app->scene->player->klastPositionX < app->scene->player->klastPosX[48])
				{
					currentAnimation = &rightAnim;
				}
				else if (app->scene->player->klastPositionX > app->scene->player->klastPosX[48])
				{
					currentAnimation = &leftAnim;
				}
				else if (app->scene->player->klastPositionY > app->scene->player->klastPosY[48])
				{
					currentAnimation = &upAnim;
				}
				else if (app->scene->player->klastPositionY < app->scene->player->klastPosY[48])
				{
					currentAnimation = &rightAnim;
				}
				else
				{
					currentAnimation = &idlAnim;

				}
			}
		}
	}
	currentAnimation->Update();
	return true;
}

bool NPC7::PostUpdate()
{
	if ((this->active == true) && (!app->scene->paused) && (!app->sceneDungeon->paused) && !app->sceneBattle->knightDead)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texNPC7, position.x, position.y, &rect);
	}
	return true;
}

bool NPC7::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texNPC7);
	return true;
}

void NPC7::NPC7InitialPosition()
{

	if (app->sceneBattle->battleOn)
	{
		position.x = 80;
		position.y = 100;
	}
	else
	{
		if (app->scene->player->knightTkn)
		{
			position.x = app->scene->player->klastPositionX;
			position.y = app->scene->player->klastPositionY - 5;
		}
		else
		{
			position.x = 2000;
			position.y = 1050;
		}
	}
}