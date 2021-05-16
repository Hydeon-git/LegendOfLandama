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

#define JOURNAL_TILE_SIZE 124

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
		journalTex = app->tex->Load("Assets/Textures/tasks.png");
		questFx = app->audio->LoadFx("Assets/Audio/Fx/open_quest.wav");
		questDoneFx = app->audio->LoadFx("Assets/Audio/Fx/quest_done.wav");
		questsIsOpen = false;

	}
	return ret;
}

bool Quests::Update(float dt)
{
	GamePad& pad = app->input->pads[0];

	if ((app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN || pad.r1 == true) && !questsIsOpen)
	{
		questsIsOpen = true;
		app->hud->bagIsOpen = false;
		app->audio->PlayFx(questFx, 0);
		app->shop->open = false;
	} 

	else if ((app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN || pad.r1 == true) && questsIsOpen)
	{
		questsIsOpen = false;
		app->audio->PlayFx(questFx, 0);
	}

	if (app->sceneBattle->enemy1Dead && app->sceneBattle->enemy2Dead && app->sceneBattle->enemy3Dead)
	{
		quest1Done = true;

	}
	
	if (app->sceneDungeon->active)
	{
		if (app->sceneDungeon->player->keyGreenDone)
		{
			quest3Done = true;
		}
	}

	if (quest1Done) 
	{
		if (counterQuest1 <= 201)
		{
			counterQuest1++;
		}
	}
	if (quest2Done)
	{
		if (counterQuest2 <= 201)
		{
			counterQuest2++;
		}
	}
	if (quest3Done)
	{
		if (counterQuest3 <= 201)
		{
			counterQuest3++;
		}
	}

	//if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	//{
	//	questsIsOpen = false;
	//	app->hud->bagIsOpen = true;
	//}

	return true;
}

// Update: draw background
bool Quests::PostUpdate()
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
	if (!questsIsOpen)
		SDL_RenderCopy(app->render->renderer, journalTex, &SrcR, &DestR);


	openBackPack();


	if (questsIsOpen)
	{
		//questsTake
		if (quest1Taken)
		{
			app->font->DrawText(145, 76, app->scene->whiteFont, "Kill 3 goblins");
		}
		if (quest2Taken)
		{
			app->font->DrawText(145, 111, app->scene->whiteFont, "Buy a Weapon");
		}
		if (quest3Taken)
		{
			app->font->DrawText(145, 147, app->scene->whiteFont, "Finish the Dungeon");
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




	if (quest1Done)
	{
		if (counterQuest1 < 2)
		{
			app->audio->PlayFx(questDoneFx, 0);
		}

		if (counterQuest1 < 200) 
		{
			app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
			app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);

			app->font->DrawText(15, 205, app->scene->whiteFont, "Quest 1 Completed");
		}
	}
	
	if (quest2Done)
	{
		if (counterQuest2 < 2)
		{
			app->audio->PlayFx(questDoneFx, 0);
		}
		if (counterQuest2 < 200)
		{
			app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
			app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);

			app->font->DrawText(15, 205, app->scene->whiteFont, "Quest 2 Completed");
		}
	}
	if (quest3Done)
	{
		if (counterQuest3 < 2)
		{
			app->audio->PlayFx(questDoneFx, 0);
		}
		if (counterQuest3 < 200)
		{
			app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
			app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);

			app->font->DrawText(15, 205, app->scene->whiteFont, "Quest 3 Completed");
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

	DestR.x = 340;
	DestR.y = 100;
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

	DestR.x = 400;
	DestR.y = 220;
	DestR.w = 40;
	DestR.h = 40;
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

	DestR.x = 400;
	DestR.y = 326;
	DestR.w = 40;
	DestR.h = 40;
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

	DestR.x = 400;
	DestR.y = 435;
	DestR.w = 40;
	DestR.h = 40;
	SDL_RenderCopy(app->render->renderer, markTex, &SrcR, &DestR);

}

