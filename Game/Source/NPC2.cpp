#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "NPC2.h"
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


NPC2::NPC2() : Entity(EntityType::NPC2)
{
	name.Create("NPC2");
	position.x = 352;
	position.y = 128;

	//idlanim
	idlAnim.PushBack({ 0, 0, 10, 19 });
	idlAnim.PushBack({ 32, 0, 10, 19 });
	idlAnim.speed = 0.02f;


	//move right
	rightAnim.PushBack({ 0, 0, 10, 19 });
	rightAnim.PushBack({ 32, 0, 10, 19 });
	rightAnim.PushBack({ 63, 0, 12, 19 });
	rightAnim.PushBack({ 96, 0, 10, 19 });
	rightAnim.PushBack({ 128, 0, 10, 19 });
	rightAnim.PushBack({ 159, 0, 12, 19 });
	rightAnim.PushBack({ 190, 0, 12, 19 });
	rightAnim.PushBack({ 224, 0, 10, 19 });

	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 236, 0, 10, 19 });
	leftAnim.PushBack({ 268, 0, 12, 19});
	leftAnim.PushBack({ 299, 0, 12, 19});
	leftAnim.PushBack({ 332, 0, 10, 19});
	leftAnim.PushBack({ 364, 0, 10,19 });
	leftAnim.PushBack({ 395, 0, 12,19 });
	leftAnim.PushBack({ 428, 0, 10,19 });
	leftAnim.PushBack({ 460, 0, 10,19 });

	leftAnim.speed = 0.1f;

	currentAnimation = &idlAnim;


}

// Destructor
NPC2::~NPC2()
{}

bool NPC2::Awake()
{
	LOG("Loading NPC2");
	bool ret = true;

	return ret;
}

bool NPC2::Start()
{
	if (this->active == true)
	{
		texNPC2 = app->tex->Load("Assets/Textures/npc2_character.png");
		//enemyDeathFx = app->audio->LoadFx("Assets/Audio/Fx/enemy_death.wav");

		currentAnimation = &idlAnim;
	}
	return true;
}

bool NPC2::Update(float dt)
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


bool NPC2::PostUpdate()
{

	if (this->active == true)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texNPC2, position.x, position.y, &rect);
	}
	return true;
}






bool NPC2::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texNPC2);
	//app->entityManager->DestroyEntity(this);
	return true;
}



void NPC2::NPC2InitialPosition()
{
	position.x = 352;
	position.y = 128;
}
