#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "Potion.h"
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


Potion::Potion() : Item(ItemType::POTION)
{
	texture = app->tex->Load("Assets/Textures/potion.png");


}

// Destructor
Potion::~Potion() {}

bool Potion::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool Potion::Start()
{
	if (this->active == true)
	{
	
	}
	return true;
}

bool Potion::Update(float dt)
{
	
	return true;
}

bool Potion::PostUpdate()
{
	
	
	return true;
}

bool Potion::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}

