#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "NPC1.h"
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


NPC1::NPC1() : Entity(EntityType::NPC1)
{
	name.Create("NPC1");
	position.x = 171;
	position.y = 405;

	//idlanim
	idlAnim.PushBack({ 0, 0, 15, 17 });
	idlAnim.PushBack({ 32, 1, 15, 16 });
	idlAnim.speed = 0.02f;


	//move right
	rightAnim.PushBack({ 0, 0, 15, 17 });
	rightAnim.PushBack({ 32, 1, 15, 16 });
	rightAnim.PushBack({ 63, 2, 16, 15 });
	rightAnim.PushBack({ 96, 1, 15, 16 });
	rightAnim.PushBack({ 128, 3, 15, 14 });
	rightAnim.PushBack({ 160, 0, 15, 17 });
	rightAnim.PushBack({ 192, 1, 16, 16 });
	rightAnim.PushBack({ 225, 4, 16, 13 });

	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 0, 21, 16, 13 });
	leftAnim.PushBack({ 33, 18, 16, 16 });
	leftAnim.PushBack({ 66, 17, 15, 17 });
	leftAnim.PushBack({ 98, 20, 15, 14 });
	leftAnim.PushBack({ 130, 18, 15, 16 });
	leftAnim.PushBack({ 162, 19, 16, 15 });
	leftAnim.PushBack({ 194, 18, 15, 16 });
	leftAnim.PushBack({ 226, 17, 15, 17 });

	leftAnim.speed = 0.1f;

	currentAnimation = &idlAnim;


}

// Destructor
NPC1::~NPC1()
{}

bool NPC1::Awake()
{
	LOG("Loading NPC1");
	bool ret = true;

	return ret;
}

bool NPC1::Start()
{
	if (this->active == true)
	{
		texNPC1 = app->tex->Load("Assets/Textures/npc1_character.png");
		//enemyDeathFx = app->audio->LoadFx("Assets/Audio/Fx/enemy_death.wav");

		currentAnimation = &idlAnim;
	}
	return true;
}

bool NPC1::Update(float dt)
{
	
	
	if (!pause)
	{
		if (right)
		{
			position.x += speed;
			currentAnimation = &rightAnim;
		}
		if (!right)
		{
			position.x -= speed;
			currentAnimation = &leftAnim;
		}

		if (position.x >= 254) right = false;
		if (position.x <= 171) right = true;

		if (position.x < 253 && position.x>170) prova = false;
		if ((position.x == 254 || position.x == 171) && !prova)
		{
			pause = true;
		}
	}
	if(pause) counter++;
	if (counter >= 50)
	{
		pause = false;
		counter = 0;
		prova = true;
	}


	currentAnimation->Update();


	//if (app->entityManager->entityList.At(0)->data->position.x > position.x - 30 &&
	//	app->entityManager->entityList.At(0)->data->position.x < position.x + 60 &&
	//	app->entityManager->entityList.At(0)->data->position.y > position.y - 46 &&
	//	app->entityManager->entityList.At(0)->data->position.y < position.y + 92)
	//{
	//	app->entityManager->entityList.At(0)->data->position.x = app->entityManager->entityList.At(0)->data->position.y;
	//}



	
	return true;
}

bool NPC1::PostUpdate()
{

	if (this->active == true)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texNPC1, position.x, position.y, &rect);
	}
	return true;
}






bool NPC1::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texNPC1);
	//app->entityManager->DestroyEntity(this);
	return true;
}



void NPC1::NPC1InitialPosition()
{
	position.x = 171;
	position.y = 405;
}