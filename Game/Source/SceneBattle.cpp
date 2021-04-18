#include "SceneBattle.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "SceneWin.h"
#include "SceneLose.h"
#include "FadeToBlack.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "Font.h"
#include "Map.h"
#include "EntityManager.h"
#include "Player.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "NPC5.h"

#include "Defs.h"
#include "Log.h"

SceneBattle::SceneBattle() : Module()
{
	name.Create("SceneBattle");
}

SceneBattle::~SceneBattle()
{

}

bool SceneBattle::Awake(pugi::xml_node& node)
{
	fullscreenRect = new SDL_Rect{ 0, 0, 1280, 720 };
	return true;
}

bool SceneBattle::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		LOG("Loading background assets");

		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		npc5 = (NPC5*)app->entityManager->CreateEntity(EntityType::NPC5);
		enemy1 = (Enemy1*)app->entityManager->CreateEntity(EntityType::Enemy1);
		enemy2 = (Enemy2*)app->entityManager->CreateEntity(EntityType::Enemy2);
		enemy3 = (Enemy3*)app->entityManager->CreateEntity(EntityType::Enemy3);

		player->Start();
		npc5->Start();
		enemy1->Start();
		enemy2->Start();
		enemy3->Start();

		
		//introText = app->tex->Load("Assets/Textures/portada.png");
		app->audio->PlayMusic("Assets/Audio/Music/battle_theme.ogg");
		battletext = app->tex->Load("Assets/Textures/battleback.png");
		app->sceneBattle->player->onBattle = true;
		app->sceneLose->Disable();
		app->sceneWin->Disable();
		app->scene->Disable();
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		battleOn = true;
		char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
		whiteFont = app->font->Load("Assets/Textures/white_font_mini.png", lookupTable, 1);
		blackFont = app->font->Load("Assets/Textures/black_font_mini.png", lookupTable, 1);

		btnAttack = new GuiButton(1, { 20, 180, 80, 15 }, "Heroine");
		btnAttack->SetObserver(this);

		btnDefense = new GuiButton(2, { 20, 200, 80, 15 }, "Mage");
		btnDefense->SetObserver(this);
	}
	return ret;
}

bool SceneBattle::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		app->fadeToBlack->FadeToBlk(this, app->scene, 30);
		battleOn = false;
		battleEnd = true;
	}
	btnAttack->Update(dt);
	btnDefense->Update(dt);
	return true;
}

// Update: draw background
bool SceneBattle::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	app->render->DrawTexture(battletext, 0, 0, fullscreenRect, 3);
	app->render->DrawRectangle({ 0, 510, 1280, 210 }, 0, 0, 0, 220, true, false);
	app->render->DrawRectangle({ 10, 520, 1260, 190 }, 100, 100, 200, 220, true, false);

	app->font->DrawText(15, 180, blackFont, "NAME");

	app->font->DrawText(110, 180, blackFont, "ACTION");

	app->font->DrawText(280, 180, blackFont, "HP");



	btnAttack->Draw();
	btnDefense->Draw();

	return ret;
}

bool SceneBattle::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1)
		{
			app->fadeToBlack->FadeToBlk(this, app->scene, 30);
			battleOn = false;
			battleEnd = true;
		}
		else if (control->id == 2)
		{
			app->fadeToBlack->FadeToBlk(this, app->scene, 30);
			battleOn = false;
			battleEnd = true;
		}

		else if (control->id == 3)
		{
			options = true;
		}
		else if (control->id == 4)
		{
			if (app->scene->player != nullptr)
			{
				app->scene->player->position.x = 350;
				app->scene->player->position.y = 875;
				app->SaveGameRequest();
			}
			exit = true;
		}
		else if (control->id == 5)
		{
			//btnOptions->state = GuiControlState::NORMAL;
			options = false;
		}
	}
	default: break;
	}

	return true;
}

bool SceneBattle::CleanUp()
{
	app->tex->UnLoad(battletext);
	app->font->UnLoad(whiteFont);
	app->entityManager->DestroyEntity(player);
	app->entityManager->DestroyEntity(enemy1);
	app->entityManager->DestroyEntity(enemy2);
	app->entityManager->DestroyEntity(enemy3);
	app->entityManager->DestroyEntity(npc5);
	return true;
}