#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "DiamondBow.h"
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


DiamondBow::DiamondBow() : Item(ItemType::DIAMOND_BOW)
{
	texture = app->tex->Load("Assets/Textures/diamond_bow.png");
	price = 20;


}

// Destructor
DiamondBow::~DiamondBow() {}

bool DiamondBow::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool DiamondBow::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool DiamondBow::Update(float dt)
{

	return true;
}

bool DiamondBow::PostUpdate()
{


	return true;
}

bool DiamondBow::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}


