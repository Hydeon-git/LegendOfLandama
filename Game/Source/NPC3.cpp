#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "NPC3.h"
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


NPC3::NPC3() : Entity(EntityType::NPC3)
{
	name.Create("NPC3");
	position.x = 200;
	position.y = 100;

	//idlanim
	idlAnim.PushBack({ 0, 0, 21, 25 });
	idlAnim.PushBack({ 32, 0, 21, 25 });
	idlAnim.speed = 0.02f;


	//move right
	rightAnim.PushBack({ 0, 0, 21, 25 });
	rightAnim.PushBack({ 32, 0, 21, 25 });
	rightAnim.PushBack({ 64, 0, 21, 25 });
	rightAnim.PushBack({ 96, 0, 21, 25 });
	rightAnim.PushBack({ 127, 0, 22, 25 });
	rightAnim.PushBack({ 159, 0, 22, 25 });
	rightAnim.PushBack({ 191, 0, 22, 25 });
	rightAnim.PushBack({ 223, 0, 22, 25 });

	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 248, 0, 22, 25 });
	leftAnim.PushBack({ 280, 0, 22, 25 });
	leftAnim.PushBack({ 312, 0, 22, 25 });
	leftAnim.PushBack({ 344, 0, 22, 25 });
	leftAnim.PushBack({ 376, 0, 21,25 });
	leftAnim.PushBack({ 408, 0, 21,25 });
	leftAnim.PushBack({ 440, 0, 21,25 });
	leftAnim.PushBack({ 472, 0, 21,25 });

	leftAnim.speed = 0.1f;

	currentAnimation = &idlAnim;


}

// Destructor
NPC3::~NPC3()
{}

bool NPC3::Awake()
{
	LOG("Loading NPC3");
	bool ret = true;

	return ret;
}

bool NPC3::Start()
{
	if (this->active == true)
	{
		texNPC3 = app->tex->Load("Assets/Textures/npc3_character.png");
		//enemyDeathFx = app->audio->LoadFx("Assets/Audio/Fx/enemy_death.wav");

		currentAnimation = &idlAnim;
	}
	return true;
}

bool NPC3::Update(float dt)
{

	currentAnimation = &idlAnim;

	/*if (!pause)
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
*/
/*if (position.x >= 254) right = false;
if (position.x <= 171) right = true;

if (position.x < 253 && position.x>170) prova = false;
if ((position.x == 254 || position.x == 171) && !prova)
{
	pause = true;
}
}
if (pause) counter++;
if (counter >= 50)
{
	pause = false;
	counter = 0;
	prova = true;
}*/


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


bool NPC3::PostUpdate()
{

	if (this->active == true)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texNPC3, position.x, position.y, &rect);
	}
	return true;
}






bool NPC3::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texNPC3);
	//app->entityManager->DestroyEntity(this);
	return true;
}



void NPC3::NPC3InitialPosition()
{
	position.x = 200;
	position.y = 100;
}
