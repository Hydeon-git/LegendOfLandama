#include "Shop.h"
#include "App.h"
#include "Render.h"

#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Font.h"
#include "Player.h"
#include <SDL_mixer\include\SDL_mixer.h>
#include <SDL\include\SDL_keyboard.h>
#include "stdio.h"

#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Font.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "FadeToBlack.h"
#include "Shop.h"
#include "HUD.h"

#include "SceneIntro.h"
#include "GuiButton.h"

#include <iostream>

#include "Defs.h"
#include "Log.h"

#define BACKPACK_TILE_SIZE 124

HUD::HUD() : Module()
{
	name.Create("HUD");
}

HUD::~HUD()
{

}

bool HUD::Awake(pugi::xml_node& node)
{
	return true;
}

bool HUD::Start()
{
	bool ret = true;
	if (this->active == true)
	{

		LOG("Loading background assets");
		bagTex = app->tex->Load("Assets/Textures/backpack.png");
		openbagTex = app->tex->Load("Assets/Textures/open_backpack.png");

		bagIsOpen = false;

	}
	return ret;
}

bool HUD::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		bagIsOpen = true;
		
	}

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		 bagIsOpen= false;
	}

	return true;
}

// Update: draw background
bool HUD::PostUpdate()
{
	bool ret = true;

	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = BACKPACK_TILE_SIZE;
	SrcR.h = BACKPACK_TILE_SIZE;

	DestR.x = 1150;
	DestR.y = -10;
	DestR.w = BACKPACK_TILE_SIZE;
	DestR.h = BACKPACK_TILE_SIZE;
	if (!bagIsOpen)
	SDL_RenderCopy(app->render->renderer, bagTex, &SrcR, &DestR);


	openBackPack();


	if (bagIsOpen)
		//inventory
		app->scene->player->drawPlayerInventory();
	
	return ret;
}


bool HUD::CleanUp()
{

	app->tex->UnLoad(bagTex);
	app->tex->UnLoad(openbagTex);

	return true;
}

void HUD::openBackPack()
{
	if (!bagIsOpen)
		return;

	app->render->DrawTexture(openbagTex, 8, 0, NULL,2);

}


