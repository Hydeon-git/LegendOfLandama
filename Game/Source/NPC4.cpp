#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "NPC4.h"
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


NPC4::NPC4() : Entity(EntityType::NPC4)
{
	name.Create("NPC4");
	position.x = 98;
	position.y = 154;

	//idlanim
	idlAnim.PushBack({ 0, 0, 12, 22 });
	idlAnim.PushBack({ 32, 0, 12, 22 });
	idlAnim.speed = 0.02f;

	idlAnimLeft.PushBack({ 238, 0, 14, 22 });
	idlAnimLeft.PushBack({ 270, 0, 14, 22 });
	idlAnimLeft.speed = 0.02f;
	
	//move right
	rightAnim.PushBack({ 0, 0, 12, 22 });
	rightAnim.PushBack({ 32, 0, 12, 22 });
	rightAnim.PushBack({ 63, 0, 13, 22 });
	rightAnim.PushBack({ 96, 0, 12, 22 });
	rightAnim.PushBack({ 126, 0, 14, 22 });
	rightAnim.PushBack({ 158, 0, 14, 22 });
	rightAnim.PushBack({ 190, 0, 14, 22 });
	rightAnim.PushBack({ 222, 0, 14, 22 });

	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 238, 0, 14, 22 });
	leftAnim.PushBack({ 270, 0, 14, 22 });
	leftAnim.PushBack({ 302, 0, 14, 22 });
	leftAnim.PushBack({ 334, 0, 14, 22 });
	leftAnim.PushBack({ 366, 0, 12,22 });
	leftAnim.PushBack({ 398, 0, 13,22 });
	leftAnim.PushBack({ 430, 0, 12,22 });
	leftAnim.PushBack({ 462, 0, 12,22 });

	leftAnim.speed = 0.1f;

	currentAnimation = &idlAnim;
}

// Destructor
NPC4::~NPC4()
{}

bool NPC4::Awake()
{
	LOG("Loading NPC4: Knight");
	bool ret = true;

	return ret;
}

bool NPC4::Start()
{
	if (this->active == true)
	{
		texNPC4 = app->tex->Load("Assets/Textures/npc4_character.png");
		currentAnimation = &idlAnim;
	}
	return true;
}

bool NPC4::Update(float dt)
{

	

	/*if (!pause && !app->scene->player->dialogeOn)

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

		if (position.x >= 250) right = false;
		if (position.x <= 141) right = true;
		if (position.x < 250 && position.x>140) prova = false;

		if ((position.x == 249 || position.x == 141) && !prova)
		{
			pause = true;
		}
	}
	if (pause)
	{
		counter++;
		if (right) currentAnimation = &idlAnim;
		if (!right) currentAnimation = &idlAnimLeft;
	}
	if (counter >= 50)
	{
		pause = false;
		counter = 0;
		prova = true;

	}*/

	currentAnimation->Update();
	return true;
}

bool NPC4::PostUpdate()
{
	if (this->active == true)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texNPC4, position.x, position.y, &rect);
	}
	return true;
}

bool NPC4::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texNPC4);
	//app->entityManager->DestroyEntity(this);
	return true;
}

void NPC4::NPC4InitialPosition()
{
	position.x = 98;
	position.y = 154;
}

