#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "GoldShield.h"
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


GoldShield::GoldShield() : Item(ItemType::GOLD_PROTECTION)
{
	texture = app->tex->Load("Assets/Textures/gold_shield.png");
	price = 12;


}

// Destructor
GoldShield::~GoldShield() {}

bool GoldShield::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool GoldShield::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool GoldShield::Update(float dt)
{

	return true;
}

bool GoldShield::PostUpdate()
{


	return true;
}

bool GoldShield::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}

