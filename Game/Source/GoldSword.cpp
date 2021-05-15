#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "GoldSword.h"
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


GoldSword::GoldSword() : Item(ItemType::GOLD_SWORD)
{
	texture = app->tex->Load("Assets/Textures/gold_sword.png");
	price = 12;

}

// Destructor
GoldSword::~GoldSword() {}

bool GoldSword::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool GoldSword::Start()
{
	if (this->active == true)
	{
		

	}
	return true;
}

bool GoldSword::Update(float dt)
{

	return true;
}

bool GoldSword::PostUpdate()
{


	return true;
}

bool GoldSword::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}

