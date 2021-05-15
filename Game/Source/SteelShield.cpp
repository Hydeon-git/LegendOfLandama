#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "SteelShield.h"
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


SteelShield::SteelShield() : Item(ItemType::STEEL_PROTECTION)
{

	texture = app->tex->Load("Assets/Textures/steel_shield.png");
	price = 8;

}

// Destructor
SteelShield::~SteelShield() {}

bool SteelShield::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool SteelShield::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool SteelShield::Update(float dt)
{

	return true;
}

bool SteelShield::PostUpdate()
{


	return true;
}

bool SteelShield::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}

