#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "WoodWand.h"
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


WoodWand::WoodWand() : Item(ItemType::Wood_Wand)
{
	texture = app->tex->Load("Assets/Textures/wood_wand.png");


}

// Destructor
WoodWand::~WoodWand() {}

bool WoodWand::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool WoodWand::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool WoodWand::Update(float dt)
{

	return true;
}

bool WoodWand::PostUpdate()
{


	return true;
}

bool WoodWand::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}


