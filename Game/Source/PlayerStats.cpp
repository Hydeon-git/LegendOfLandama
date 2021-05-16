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
#include "NPC5.h"
#include "NPC7.h"
#include "PlayerStats.h"

#include "SceneBattle.h"
#include "SceneDungeon.h"
#include "SceneIntro.h"
#include "GuiButton.h"

#include <iostream>

#include "Defs.h"
#include "Log.h"

#define JOURNAL_TILE_SIZE 124

PlayerStats::PlayerStats() : Module()
{
	name.Create("PlayerStats");
}

PlayerStats::~PlayerStats()
{

}

bool PlayerStats::Awake(pugi::xml_node& node)
{
	return true;
}

bool PlayerStats::Start()
{
	bool ret = true;
	if (this->active == true)
	{

		LOG("Loading background assets");
		statsTex = app->tex->Load("Assets/Textures/stats.png");
		//markTex = app->tex->Load("Assets/Textures/quest_done.png");
		journalTex = app->tex->Load("Assets/Textures/tasks.png");
		heroineTex = app->tex->Load("Assets/Textures/maincharacter.png");
		mageTex = app->tex->Load("Assets/Textures/mage.png");
		knightTex = app->tex->Load("Assets/Textures/knight.png");
		questFx = app->audio->LoadFx("Assets/Audio/Fx/open_quest.wav");
		questDoneFx = app->audio->LoadFx("Assets/Audio/Fx/quest_done.wav");
		statsIsOpen = false;

	}
	return ret;
}

bool PlayerStats::Update(float dt)
{
	GamePad& pad = app->input->pads[0];

	if ((app->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN || pad.r1 == true) && !statsIsOpen)
	{
		statsIsOpen = true;
		app->hud->bagIsOpen = false;
		app->audio->PlayFx(questFx, 0);
		app->shop->open = false;
	}

	else if ((app->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN || pad.r1 == true) && statsIsOpen)
	{
		statsIsOpen = false;
		app->audio->PlayFx(questFx, 0);
	}

	return true;
}

// Update: draw background
bool PlayerStats::PostUpdate()
{
	bool ret = true;


	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = JOURNAL_TILE_SIZE;
	SrcR.h = JOURNAL_TILE_SIZE;

	DestR.x = 1150;
	DestR.y = 80;
	DestR.w = JOURNAL_TILE_SIZE;
	DestR.h = JOURNAL_TILE_SIZE;
	if (!statsIsOpen)
		SDL_RenderCopy(app->render->renderer, journalTex, &SrcR, &DestR);


	openBackPack();


	if (statsIsOpen)
	{
		//questsTake
		app->font->DrawText(135, 85, app->scene->whiteFont, "Heroine");
		drawHealthHeroine();

		if (app->scene->mageTkn)
		{
			app->font->DrawText(135, 120, app->scene->whiteFont, "Mage");
			drawHealthMage();
		}
		if (app->scene->knightTkn)
		{
			app->font->DrawText(135, 155, app->scene->whiteFont, "Knight");
			drawHealthKnight();
		}
	}

	return ret;
}


bool PlayerStats::CleanUp()
{

	app->tex->UnLoad(statsTex);

	return true;
}



void PlayerStats::openBackPack()
{
	if (!statsIsOpen)
		return;

	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 1280;
	SrcR.h = 720;

	DestR.x = 340;
	DestR.y = 100;
	DestR.w = 600;
	DestR.h = 467;
	SDL_RenderCopy(app->render->renderer, statsTex, &SrcR, &DestR);
}



void PlayerStats::drawHealthHeroine()
{

	std::string r = std::to_string(app->scene->player->playerHealth);
	char const* rchar = r.c_str();

	std::string x = std::to_string(app->scene->player->playerDmg);
	char const* xchar = x.c_str();

	std::string y = std::to_string(app->scene->player->playerDmg);
	char const* ychar = y.c_str();

	app->font->DrawText(215, 85, app->scene->whiteFont, rchar);

	app->font->DrawText(250, 85, app->scene->whiteFont, xchar);

	app->font->DrawText(290, 85, app->scene->whiteFont, ychar);

	app->font->DrawText(285, 85, app->scene->whiteFont, "/");

	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 1280;
	SrcR.h = 720;

	DestR.x = 360;
	DestR.y = 240;
	DestR.w = 46;
	DestR.h = 60;
	SDL_RenderCopy(app->render->renderer, heroineTex, &SrcR, &DestR);

}

void PlayerStats::drawHealthMage()
{

	std::string r = std::to_string(app->scene->npc5->mageHealth);
	char const* rchar = r.c_str();

	std::string x = std::to_string(app->scene->npc5->mageDmg);
	char const* xchar = x.c_str();

	std::string y = std::to_string(app->scene->npc5->mageDmg);
	char const* ychar = y.c_str();

	app->font->DrawText(215, 120, app->scene->whiteFont, rchar);

	app->font->DrawText(250, 120, app->scene->whiteFont, xchar);

	app->font->DrawText(290, 120, app->scene->whiteFont, ychar);

	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 1280;
	SrcR.h = 720;

	DestR.x = 360;
	DestR.y = 350;
	DestR.w = 46;
	DestR.h = 60;
	SDL_RenderCopy(app->render->renderer, mageTex, &SrcR, &DestR);

}

void PlayerStats::drawHealthKnight()
{

	std::string r = std::to_string(app->scene->npc7->knightHealth);
	char const* rchar = r.c_str();

	std::string x = std::to_string(app->scene->npc7->knightDmg);
	char const* xchar = x.c_str();

	std::string y = std::to_string(app->scene->npc7->knightMagicDmg);
	char const* ychar = x.c_str();

	app->font->DrawText(215, 155, app->scene->whiteFont, rchar);

	app->font->DrawText(250, 155, app->scene->whiteFont, xchar);

	app->font->DrawText(290, 155, app->scene->whiteFont, ychar);

	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 1280;
	SrcR.h = 720;

	DestR.x = 360;
	DestR.y = 475;
	DestR.w = 46;
	DestR.h = 60;
	SDL_RenderCopy(app->render->renderer, knightTex, &SrcR, &DestR);

}