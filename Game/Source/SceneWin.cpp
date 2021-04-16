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
#include "SceneWin.h"
#include "SceneIntro.h"
#include "GuiButton.h"

#include "Defs.h"
#include "Log.h"

SceneWin::SceneWin() : Module()
{
	name.Create("SceneWin");
}

SceneWin::~SceneWin()
{

}

bool SceneWin::Awake(pugi::xml_node& node)
{
	return true;
}

bool SceneWin::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		LOG("Loading background assets");
		winText = app->tex->Load("Assets/Textures/scene_win.png");
		char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
		whiteFont = app->font->Load("Assets/Textures/white_font.png", lookupTable, 1);
		app->scene->player->active = false;
		app->scene->enemy->active = false;
		app->scene->flyingEnemy->active = false;
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
	}
	return ret;
}

bool SceneWin::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) guiColliders = !guiColliders;
	btnRestart->Update(dt);
	btnExit->Update(dt);
	return true;
}

// Update: draw background
bool SceneWin::PostUpdate()
{
	bool ret = true;
	if (exit==true) ret = false;

	app->render->DrawTexture(winText, 0, 555, NULL);
	btnRestart->Draw();
	btnExit->Draw();
	return ret;
}

bool SceneWin::OnGuiMouseClickEvent(GuiControl* control)
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

bool SceneWin::CleanUp()
{
	app->font->UnLoad(whiteFont);
	app->tex->UnLoad(winText);
	return true;
}