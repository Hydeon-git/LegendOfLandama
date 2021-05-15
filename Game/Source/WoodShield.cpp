#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "WoodShield.h"
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


WoodShield::WoodShield() : Item(ItemType::WOOD_PROTECTION)
{

	texture = app->tex->Load("Assets/Textures/wood_shield.png");
	price = 5;
}

// Destructor
WoodShield::~WoodShield() {}

bool WoodShield::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool WoodShield::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool WoodShield::Update(float dt)
{

	return true;
}

bool WoodShield::PostUpdate()
{


	return true;
}

bool WoodShield::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}

