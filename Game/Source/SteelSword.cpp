#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "SteelSword.h"
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


SteelSword::SteelSword() : Item(ItemType::STEEL_SWORD)
{

	texture = app->tex->Load("Assets/Textures/steel_sword.png");
	price = 8;
}

// Destructor
SteelSword::~SteelSword() {}

bool SteelSword::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool SteelSword::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool SteelSword::Update(float dt)
{

	return true;
}

bool SteelSword::PostUpdate()
{


	return true;
}

bool SteelSword::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}

