#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "Coin.h"
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


Coin::Coin(iPoint pos) : Item(ItemType::COIN)
{


}

// Destructor
Coin::~Coin() {}

bool Coin::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool Coin::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool Coin::Update(float dt)
{

	return true;
}

bool Coin::PostUpdate()
{


	return true;
}

bool Coin::CleanUp()
{
	LOG("Freeing scene");
	//app->tex->UnLoad(texture);
	return true;
}

