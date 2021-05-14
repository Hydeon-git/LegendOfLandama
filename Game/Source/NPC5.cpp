#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "SceneBattle.h"
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
	if (app->sceneBattle->battleOn)
	{
		position.x = 120;
		position.y = 80;
	}
	else if (app->sceneBattle->mageRevive)
	{
		position.x = 128;
		position.y = 134;
		app->scene->player->mageTkn = false;
	}
	else {
		//position.x = app->scene->player->lastPositionX;
		//position.y = app->scene->player->lastPositionY - 5;
		/*position.x = 200-30;
		position.y = 100;*/
		position.x = 265;
		position.y = 319;

	}
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
	leftAnim.PushBack({ 178,0, 15, 23 });
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
	LOG("Loading NPC5: Mage");

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
	
	if (app->scene->player->mageTkn)
	{
		if (!app->sceneBattle->battleOn)
		{

			if (app->scene->player->doorTaked2)
			{
				app->scene->player->posMoved = 0;
				app->scene->player->doorTaked2 = false;
			}


			//if ((position.y > app->scene->player->position.y + 24-5 || position.y < app->scene->player->position.y - 24-5))
			//{
			//	position.y = app->scene->player->position.y - 5;
			//	app->scene->player->doorTaked = false;

			//	//app->scene->player->doIt = true;
			//}
			//else


				//movement
			if (app->scene->player->posMoved < 24)
			{
				if (app->scene->player->lastPositionX != 0) position.x = app->scene->player->lastPositionX;
				if (app->scene->player->lastPositionY != 0) position.y = app->scene->player->lastPositionY - 5;
			}




			if (app->scene->player->doorTaked)
			{
				position.x = app->scene->player->lastPositionX2;
				position.y = app->scene->player->lastPositionY2 - 5;
				if (app->scene->player->posMoved >= 24)
				{
					app->scene->player->posMoved = 0;
					app->scene->player->doorTaked = false;
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
			if (!app->scene->player->doorTaked)
			{
				if (app->scene->player->lastPositionX < app->scene->player->lastPosX[23])
				{
					currentAnimation = &rightAnim;
				}
				else if (app->scene->player->lastPositionX > app->scene->player->lastPosX[23])
				{
					currentAnimation = &leftAnim;
				}
				else if (app->scene->player->lastPositionY > app->scene->player->lastPosY[23])
				{
					currentAnimation = &upAnim;
				}
				else if (app->scene->player->lastPositionY < app->scene->player->lastPosY[23])
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

bool NPC5::PostUpdate()
{
	if ((this->active == true) && (!app->scene->paused) && !app->sceneBattle->mageDead)
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

	if (app->sceneBattle->battleOn)
	{
		position.x = 100;
		position.y = 100;
	}
	else 
	{
		if (app->scene->player->mageTkn)
		{
			position.x = app->scene->player->lastPositionX;
			position.y = app->scene->player->lastPositionY - 5;
		}
		else
		{
			position.x = 2000;
			position.y = 1050;
		}
	}
}