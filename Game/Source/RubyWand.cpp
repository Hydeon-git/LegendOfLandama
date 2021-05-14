#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "RubyWand.h"
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


RubyWand::RubyWand() : Item(ItemType::Ruby_Wand)
{
	texture = app->tex->Load("Assets/Textures/rubi_wand.png");


}

// Destructor
RubyWand::~RubyWand() {}

bool RubyWand::Awake()
{
	LOG("Loading Potion: Life Potion");

	bool ret = true;
	return ret;
}

bool RubyWand::Start()
{
	if (this->active == true)
	{

	}
	return true;
}

bool RubyWand::Update(float dt)
{

	return true;
}

bool RubyWand::PostUpdate()
{


	return true;
}

bool RubyWand::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texture);
	return true;
}


