#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "DiamondWand.h"
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


DiamondWand::DiamondWand() : Item(ItemType::Diamond_Wand)
{
	texture = app->tex->Load("Assets/Textures/diamond_wand.png");


}

// Destructor
DiamondWand::~DiamondWand() {}

bool DiamondWand::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool DiamondWand::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool DiamondWand::Update(float dt)
{

	return true;
}

bool DiamondWand::PostUpdate()
{


	return true;
}

bool DiamondWand::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}


