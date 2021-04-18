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
	idlAnim.PushBack({ 1, 0, 15, 17 });
	idlAnim.PushBack({ 33, 0, 15, 17 });
	idlAnim.speed = 0.02f;

	//move right
	rightAnim.PushBack({ 1, 0, 15, 17 });
	rightAnim.PushBack({ 33, 0, 15, 17});
	rightAnim.PushBack({ 64, 0, 16, 17});
	rightAnim.PushBack({ 97, 0, 15, 17});
	rightAnim.PushBack({ 129, 0, 15,17 });
	rightAnim.PushBack({ 161, 0, 15,17 });
	rightAnim.PushBack({ 193, 0, 16,17 });
	rightAnim.PushBack({ 226, 0, 16,17 });

	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 243, 0,16, 17 });
	leftAnim.PushBack({ 276, 0, 16,17 });
	leftAnim.PushBack({ 309,0, 15, 17 });
	leftAnim.PushBack({ 341,0, 15, 17 });
	leftAnim.PushBack({ 373,0, 15, 17 });
	leftAnim.PushBack({ 405,0, 16, 17 });
	leftAnim.PushBack({ 437,0, 15, 17 });
	leftAnim.PushBack({ 469,0, 15, 17 });

	leftAnim.speed = 0.1f;

	currentAnimation = &idlAnim;
}

// Destructor
NPC1::~NPC1() {}

bool NPC1::Awake()
{
	LOG("Loading NPC1: Beach Girl");

	bool ret = true;
	return ret;
}

bool NPC1::Start()
{
	if (this->active == true)
	{
		texNPC1 = app->tex->Load("Assets/Textures/npc1_character.png");

		currentAnimation = &idlAnim;
	}
	return true;
}

bool NPC1::Update(float dt)
{	
	if (!pause && !app->scene->player->dialogeOn)
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
	return true;
}

bool NPC1::PostUpdate()
{
	if ((this->active == true) && (!app->scene->paused) && (!app->scene->houseClosed))
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
	return true;
}

void NPC1::NPC1InitialPosition()
{
	position.x = 171;
	position.y = 405;
}