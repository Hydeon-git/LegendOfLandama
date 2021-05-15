#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "SteelBow.h"
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


SteelBow::SteelBow() : Item(ItemType::STEEL_BOW)
{
	texture = app->tex->Load("Assets/Textures/steel_bow.png");
	price = 8;


}

// Destructor
SteelBow::~SteelBow() {}

bool SteelBow::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool SteelBow::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool SteelBow::Update(float dt)
{

	return true;
}

bool SteelBow::PostUpdate()
{


	return true;
}

bool SteelBow::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}


