#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "NPC1.h"
#include "NPC2.h"
#include "NPC3.h"
#include "NPC4.h"
#include "NPC5.h"
#include "NPC7.h"
#include "EntityManager.h"
#include "FadeToBlack.h"
#include "PathFinding.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "Defs.h"
#include "Log.h"
#include "Font.h"
#include "SceneDungeon.h"


SceneDungeon::SceneDungeon() : Module()
{
	name.Create("SceneDungeon");
}

SceneDungeon::~SceneDungeon()
{

}

bool SceneDungeon::Awake(pugi::xml_node& node)
{
	return true;
}

bool SceneDungeon::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		LOG("Loading background assets");
		winText = app->tex->Load("Assets/Textures/scene_win.png");
		char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
		whiteFont = app->font->Load("Assets/Textures/white_font.png", lookupTable, 1);
		app->scene->player->active = false;
		app->map->Disable();
		app->audio->PlayMusic("Assets/Audio/Music/win.ogg");
		app->map->checkpointTaken = false;
		app->scene->player->dead = false;
		app->render->camera.x = 0;
		app->render->camera.y = -556;
		btnRestart = new GuiButton(1, { 490, 1030, 300, 50 }, " RESTART");
		btnRestart->SetObserver(this);
		btnExit = new GuiButton(2, { 1113, 582, 145, 50 }, "EXIT");
		btnExit->SetObserver(this);
		won = true;

		if (app->map->Load("dungeonHall.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}
	}
	return ret;
}

bool SceneDungeon::Update(float dt)
{
	btnRestart->Update(dt);
	btnExit->Update(dt);
	return true;
}

// Update: draw background
bool SceneDungeon::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	app->map->Draw();
	app->map->DrawColliders();
	//app->render->DrawTexture(winText, 0, 555, NULL);
	//app->render->DrawRectangle({ 0, 0, 3000, 3000 }, 100, 100, 200, 220, true, false);
	btnRestart->Draw();
	btnExit->Draw();
	return ret;
}

bool SceneDungeon::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) app->fadeToBlack->FadeToBlk(this, app->sceneIntro, 30);
		else if (control->id == 2) exit = true;
	}
	default: break;
	}

	return true;
}

bool SceneDungeon::CleanUp()
{
	app->font->UnLoad(whiteFont);
	app->tex->UnLoad(winText);
	return true;
}