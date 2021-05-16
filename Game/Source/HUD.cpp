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
#include <string>
#include "SceneDungeon.h"

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
#include "Quests.h"
#include "PlayerStats.h"

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
		coinTex = app->tex->Load("Assets/Textures/coin.png");
		hudFx = app->audio->LoadFx("Assets/Audio/Fx/inventory.wav");
		coins = 20;

		bagIsOpen = false;

	}
	return ret;
}

bool HUD::Update(float dt)
{
	GamePad& pad = app->input->pads[0];

	if ((app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN || pad.y == true) && !bagIsOpen && !app->scene->paused && !app->sceneDungeon->paused)
	{
		bagIsOpen = true;
		app->quests->questsIsOpen = false;
		app->playerStats->statsIsOpen = false;
		app->audio->PlayFx(hudFx, 0);
		app->shop->open = false;
	}

	else if ((app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN || pad.y == true) && bagIsOpen && !app->scene->paused && !app->sceneDungeon->paused)
	{
		bagIsOpen = false;
		app->audio->PlayFx(hudFx, 0);
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
	{
		//inventory
		app->scene->player->drawPlayerInventory();

		drawCoins();

	}
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

	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 1280;
	SrcR.h = 720;

	DestR.x = 8;
	DestR.y = 0;
	DestR.w = 1280;
	DestR.h = 720;
	SDL_RenderCopy(app->render->renderer, openbagTex, &SrcR, &DestR);
}

void HUD::drawCoins()
{

	std::string r = std::to_string(coins);
	char const* rchar = r.c_str();



	app->font->DrawText(290, 152, app->scene->whiteFont, rchar);
	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 1280;
	SrcR.h = 720;

	DestR.x = 874;
	DestR.y = 400;
	DestR.w = 48;
	DestR.h = 48;
	SDL_RenderCopy(app->render->renderer, coinTex, &SrcR, &DestR);

}