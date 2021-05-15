#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "FireSword.h"
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


FireSword::FireSword() : Item(ItemType::FIRE_SWORD)
{
	texture = app->tex->Load("Assets/Textures/fire_sword.png");
	price = 20;


}

// Destructor
FireSword::~FireSword() {}

bool FireSword::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool FireSword::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool FireSword::Update(float dt)
{

	return true;
}

bool FireSword::PostUpdate()
{


	return true;
}

bool FireSword::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}

