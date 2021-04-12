#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Font.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "GuiButton.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "SceneLose.h"
#include "SceneIntro.h"

#include "Defs.h"
#include "Log.h"

SceneLose::SceneLose() : Module()
{
	name.Create("SceneLose");
}

SceneLose::~SceneLose(){}

bool SceneLose::Awake(pugi::xml_node& node)
{
	return true;
}

bool SceneLose::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		LOG("Loading background assets");
		loseText = app->tex->Load("Assets/Textures/scene_lose.png");
		char lookupTable[] = { "! #$%& ()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
		whiteFont = app->font->Load("Assets/Textures/white_font.png", lookupTable, 1);
		app->map->Disable();
		app->audio->PlayMusic("Assets/Audio/Music/game_over.ogg");
		app->map->checkpointTaken = false;
		app->render->camera.x = 0;
		app->render->camera.y = -556;
		btnRestart = new GuiButton(1, { 490, 1030, 300, 50 }, " RESTART");
		btnRestart->SetObserver(this);
		btnExit = new GuiButton(2, { 1113, 582, 145, 50 }, "EXIT");
		btnExit->SetObserver(this);
		lost = true;
	}
	

	return ret;
}

bool SceneLose::Update(float dt)
{	
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) guiColliders = !guiColliders;
	btnRestart->Update(dt);
	btnExit->Update(dt);
	return true;
}

// Update: draw background
bool SceneLose::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	app->render->DrawTexture(loseText, 0, 555, NULL);

	btnRestart->Draw();
	btnExit->Draw();
	return ret;
}

bool SceneLose::OnGuiMouseClickEvent(GuiControl* control)
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

bool SceneLose::CleanUp()
{
	app->font->UnLoad(whiteFont);
	app->tex->UnLoad(loseText);
	return true;
}