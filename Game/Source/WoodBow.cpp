#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "WoodBow.h"
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


WoodBow::WoodBow() : Item(ItemType::WOOD_BOW)
{
	texture = app->tex->Load("Assets/Textures/wood_bow.png");
	price = 5;

}

// Destructor
WoodBow::~WoodBow() {}

bool WoodBow::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool WoodBow::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool WoodBow::Update(float dt)
{

	return true;
}

bool WoodBow::PostUpdate()
{


	return true;
}

bool WoodBow::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}


