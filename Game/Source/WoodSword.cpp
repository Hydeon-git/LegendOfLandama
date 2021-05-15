#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "WoodSword.h"
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


WoodSword::WoodSword() : Item(ItemType::WOOD_SWORD)
{
	texture = app->tex->Load("Assets/Textures/wood_sword.png");
	price = 5;

}

// Destructor
WoodSword::~WoodSword() {}

bool WoodSword::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool WoodSword::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool WoodSword::Update(float dt)
{

	return true;
}

bool WoodSword::PostUpdate()
{


	return true;
}

bool WoodSword::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}

 
