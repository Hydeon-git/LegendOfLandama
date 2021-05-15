#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "FireShield.h"
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


FireShield::FireShield() : Item(ItemType::FIRE_PROTECTION)
{

	texture = app->tex->Load("Assets/Textures/fire_shield.png");
	price = 20;

}

// Destructor
FireShield::~FireShield() {}

bool FireShield::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool FireShield::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool FireShield::Update(float dt)
{

	return true;
}

bool FireShield::PostUpdate()
{


	return true;
}

bool FireShield::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}

