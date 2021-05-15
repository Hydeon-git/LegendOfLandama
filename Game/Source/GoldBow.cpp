#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "GoldBow.h"
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


GoldBow::GoldBow() : Item(ItemType::GOLD_BOW)
{
	texture = app->tex->Load("Assets/Textures/gold_bow.png");
	price = 12;


}

// Destructor
GoldBow::~GoldBow() {}

bool GoldBow::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool GoldBow::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool GoldBow::Update(float dt)
{

	return true;
}

bool GoldBow::PostUpdate()
{


	return true;
}

bool GoldBow::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}


