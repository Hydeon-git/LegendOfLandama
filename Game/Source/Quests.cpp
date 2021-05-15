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

#include "SceneBattle.h"
#include "SceneDungeon.h"
#include "SceneIntro.h"
#include "GuiButton.h"

#include <iostream>

#include "Defs.h"
#include "Log.h"


Quests::Quests() : Module()
{
	name.Create("Quests");
}

Quests::~Quests()
{

}

bool Quests::Awake(pugi::xml_node& node)
{
	return true;
}

bool Quests::Start()
{
	bool ret = true;
	if (this->active == true)
	{

		LOG("Loading background assets");
		questsTex = app->tex->Load("Assets/Textures/quests.png");
		markTex = app->tex->Load("Assets/Textures/quest_done.png");

		questsIsOpen = false;

	}
	return ret;
}

bool Quests::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		questsIsOpen = true;
		app->shop->open = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		questsIsOpen = false;
	}

	if (app->sceneBattle->enemy1Dead && app->sceneBattle->enemy2Dead && app->sceneBattle->enemy3Dead)
	{
		quest1Done = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		quest2Done = true;
	}
	if (app->sceneDungeon->active)
	{
		if(app->sceneDungeon->player->keyGreenDone) quest3Done = true;
	}

	return true;
}

// Update: draw background
bool Quests::PostUpdate()
{
	bool ret = true;

	openBackPack();


	if (questsIsOpen)
	{
		//questsTake
		if (quest1Taken)
		{
			app->font->DrawText(35, 45, app->scene->whiteFont, "Kill 3 goblins");
		}
		if (quest2Taken)
		{
			app->font->DrawText(35, 70, app->scene->whiteFont, "Buy a Weapon");
		}
		if (quest3Taken)
		{
			app->font->DrawText(35, 95, app->scene->whiteFont, "Finish the Dungeon");
		}

		//questsDone
		if (quest1Done)
		{
			drawMark1();
		}
		if (quest2Done)
		{
			drawMark2();
		}
		if (quest3Done)
		{
			drawMark3();
		}

	}
	return ret;
}


bool Quests::CleanUp()
{

	app->tex->UnLoad(questsTex);

	return true;
}

void Quests::openBackPack()
{
	if (!questsIsOpen)
		return;

	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 1280;
	SrcR.h = 720;

	DestR.x = 8;
	DestR.y = 0;
	DestR.w = 600;
	DestR.h = 467;
	SDL_RenderCopy(app->render->renderer, questsTex, &SrcR, &DestR);
}

void Quests::drawMark1()
{

	//std::string r = std::to_string(app->scene->player->coins);
	//char const* rchar = r.c_str();


	//app->font->DrawText(290, 152, app->scene->whiteFont, rchar);
	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 1280;
	SrcR.h = 720;

	DestR.x = 66;
	DestR.y = 142;
	DestR.w = 27;
	DestR.h = 24;
	SDL_RenderCopy(app->render->renderer, markTex, &SrcR, &DestR);

}
void Quests::drawMark2()
{

	//std::string r = std::to_string(app->scene->player->coins);
	//char const* rchar = r.c_str();


	//app->font->DrawText(290, 152, app->scene->whiteFont, rchar);
	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 1280;
	SrcR.h = 720;

	DestR.x = 66;
	DestR.y = 210;
	DestR.w = 27;
	DestR.h = 24;
	SDL_RenderCopy(app->render->renderer, markTex, &SrcR, &DestR);

}
void Quests::drawMark3()
{

	//std::string r = std::to_string(app->scene->player->coins);
	//char const* rchar = r.c_str();


	//app->font->DrawText(290, 152, app->scene->whiteFont, rchar);
	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 1280;
	SrcR.h = 720;

	DestR.x = 66;
	DestR.y = 280;
	DestR.w = 27;
	DestR.h = 24;
	SDL_RenderCopy(app->render->renderer, markTex, &SrcR, &DestR);

}

