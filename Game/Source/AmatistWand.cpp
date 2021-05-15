#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "AmatistWand.h"
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


AmatistWand::AmatistWand() : Item(ItemType::AMATIST_WAND)
{
	texture = app->tex->Load("Assets/Textures/amatist_wand.png");
	price = 18;


}

// Destructor
AmatistWand::~AmatistWand() {}

bool AmatistWand::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool AmatistWand::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool AmatistWand::Update(float dt)
{

	return true;
}

bool AmatistWand::PostUpdate()
{


	return true;
}

bool AmatistWand::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}


