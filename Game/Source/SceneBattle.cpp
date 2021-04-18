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
		goldFont = app->font->Load("Assets/Textures/gold_font_mini.png", lookupTable, 1);

		btnHeroine = new GuiButton(1, { 25, 200, 60, 15 }, "Heroine");
		btnHeroine->SetObserver(this);
		btnMage = new GuiButton(2, { 25, 220, 60, 15 }, "Mage");
		btnMage->SetObserver(this);
		btnAttack = new GuiButton(3, { 140, 180, 60, 15 }, "Attack");
		btnAttack->SetObserver(this);
		btnMagic = new GuiButton(4, { 140, 200, 60, 15 }, "Magic");
		btnMagic->SetObserver(this);
		btnDefense = new GuiButton(5, { 140, 220, 60, 15 }, "Defense");
		btnDefense->SetObserver(this);
		
		btnEnemy1 = new GuiButton(6, { 300, 100, 16, 16 }, "");
		btnEnemy1->SetObserver(this);
		btnEnemy2 = new GuiButton(7, { 315, 80, 16, 16 }, "");
		btnEnemy2->SetObserver(this);
		btnEnemy3 = new GuiButton(8, { 315, 120, 16, 16 }, "");
		btnEnemy3->SetObserver(this);

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
	btnHeroine->Update(dt);
	btnMage->Update(dt);
	if (heroine || mage)
	{
		btnAttack->Update(dt);
		btnMagic->Update(dt);
		btnDefense->Update(dt);
	}
	if (attack || magic)
	{
		btnEnemy1->Update(dt);
		btnEnemy2->Update(dt);
		btnEnemy3->Update(dt);
	}
	return true;
}

// Update: draw background
bool SceneBattle::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	app->render->DrawTexture(battletext, 0, 0, fullscreenRect, 3);
	app->render->DrawRectangle({ 0, 510, 1280, 210 }, 0, 0, 0, 220, true, false);
	app->render->DrawRectangle({ 10, 520, 330, 190 }, 100, 100, 200, 220, true, false);
	app->render->DrawRectangle({ 350, 520, 370, 190 }, 100, 100, 200, 220, true, false);
	app->render->DrawRectangle({ 730, 520, 540, 190 }, 100, 100, 200, 220, true, false);


	app->font->DrawText(35, 180, goldFont, "NAME");

	//app->font->DrawText(110, 180, goldFont, "ACTION");

	app->font->DrawText(280, 180, goldFont, "HP");


	btnHeroine->Draw();
	btnMage->Draw();
	if (heroine || mage)
	{
		btnAttack->Draw();
		btnMagic->Draw();
		btnDefense->Draw();
	}
	if (attack || magic)
	{
		btnEnemy1->Draw();
		btnEnemy2->Draw();
		btnEnemy3->Draw();
	}
	
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
			heroine = true;
			mage = false;
			btnHeroine->state = GuiControlState::DISABLED;
			btnMage->state = GuiControlState::NORMAL;
		}
		else if (control->id == 2)
		{
			mage = true;
			heroine = false;
			btnMage->state = GuiControlState::DISABLED;
			btnHeroine->state = GuiControlState::NORMAL;
		}

		else if (control->id == 3)
		{
			attack = true;
			magic = false;
			defense = false;
			btnAttack->state = GuiControlState::DISABLED;
			btnMagic->state = GuiControlState::NORMAL;
			btnDefense->state = GuiControlState::NORMAL;
		}
		else if (control->id == 4)
		{
			magic = true;
			attack = false;
			defense = false;
			btnAttack->state = GuiControlState::NORMAL;
			btnMagic->state = GuiControlState::DISABLED;
			btnDefense->state = GuiControlState::NORMAL;
		}
		else if (control->id == 5)
		{
			defense = true;
			attack = false;
			magic = false;
			btnAttack->state = GuiControlState::NORMAL;
			btnMagic->state = GuiControlState::NORMAL;
			btnDefense->state = GuiControlState::DISABLED;

		}
		else if (control->id == 6)
		{
			attack = true;
		}
		else if (control->id == 7)
		{
			magic = true;
		}
		else if (control->id == 8)
		{
			defense = true;
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