#include "SceneIntro.h"
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
#include "SceneBattle.h"
#include "FadeToBlack.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "Font.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro() : Module()
{
	name.Create("SceneIntro");
}

SceneIntro::~SceneIntro()
{

}

bool SceneIntro::Awake(pugi::xml_node& node)
{
	fullscreenRect = new SDL_Rect{ 0, 0, 1280, 720 };
	return true;
}

bool SceneIntro::Start()
{
	LOG("Loading background assets");
	bool ret = true;

	introText = app->tex->Load("Assets/Textures/portada.png");
	logoText = app->tex->Load("Assets/Textures/logo.png");
	app->sceneLose->Disable();
	app->sceneWin->Disable();
	app->audio->PlayMusic("Assets/Audio/Music/start.ogg");
	logoFx = app->audio->LoadFx("Assets/Audio/Fx/logoFx.wav");
	app->render->camera.x = 0;
	app->render->camera.y = -555;

	char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
	whiteFont = app->font->Load("Assets/Textures/black_font_mini.png", lookupTable, 1);

	btnStart = new GuiButton(1, { 50, 330, 45, 10 }, "START");
	btnStart->SetObserver(this);
	
	btnContinue = new GuiButton(2, { 60, 350, 72, 10 }, "CONTINUE");
	btnContinue->SetObserver(this);
	
	btnOptions = new GuiButton(3, { 70, 370, 64, 10 }, "OPTIONS");
	btnOptions->SetObserver(this);
	
	btnExit = new GuiButton(4, { 80, 390, 36, 10 }, "EXIT");
	btnExit->SetObserver(this);
	
	btnBackOptions = new GuiButton(5, { 120, 400, 36, 10 }, "BACK");
	btnBackOptions->SetObserver(this);

	//sliderMusicVolume = new GuiSlider(1, { 200, 300, 2, 5 }, "MUSIC VOLUME");
	//sliderMusicVolume->SetObserver(this);

	//sliderFxVolume = new GuiSlider(2, { 200, 330, 2, 5 }, " FX VOLUME");
	//sliderFxVolume->SetObserver(this);

	checkBoxFullscreen = new GuiCheckBox(1, { 190, 340, 15, 15 }, "FULLSCREEN");
	checkBoxFullscreen->SetObserver(this);

	/*checkBoxVSync = new GuiCheckBox(2, { 225,370,10,10 }, "   VSYNC");
	checkBoxVSync->SetObserver(this);*/

	return ret;
}

bool SceneIntro::Update(float dt)
{

	if (count <= 200) 
	{
		count++;
		app->audio->PlayFx(logoFx, 0);
	}
	else if (count > 200)
	{		
		if (options == true)
		{
			//sliderMusicVolume->Update(dt);
			//sliderFxVolume->Update(dt);
			btnBackOptions->Update(dt);
			checkBoxFullscreen->Update(dt);
			//checkBoxVSync->Update(dt);
		}
		else
		{
			btnStart->Update(dt);
			btnContinue->Update(dt);
			btnOptions->Update(dt);
			btnExit->Update(dt);
		}
	}
	return true;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	if (count <= 200) app->render->DrawTexture(logoText, 0, 555, fullscreenRect, 3);

	else if (count > 200)
	{
		app->render->DrawTexture(introText, 0, 555, fullscreenRect, 3);

		if (options == true)
		{
			//sliderMusicVolume->Draw();
			//sliderFxVolume->Draw();
			btnBackOptions->Draw();
			//btnExit->Draw();
			checkBoxFullscreen->Draw();
			//checkBoxVSync->Draw();
		}
		else
		{
			btnStart->Draw();
			btnContinue->Draw();
			if (!posContinue || app->sceneWin->won || app->sceneLose->lost) btnContinue->state = GuiControlState::DISABLED;
			else if (posContinue) btnContinue->state = GuiControlState::NORMAL;
			btnOptions->Draw();
			btnExit->Draw();
		}
	}

	return ret;
}

bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1)
		{
			app->fadeToBlack->FadeToBlk(this, app->scene, 30);
			startClicked = true;
			app->sceneBattle->enemy1Dead = false;
			app->sceneBattle->enemy2Dead = false;
			app->sceneBattle->enemy3Dead = false;
			app->sceneBattle->enemiesAlive = 3;
		}
		else if (control->id == 2) app->fadeToBlack->FadeToBlk(this, app->scene, 30);

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
			btnOptions->state = GuiControlState::NORMAL;
			options = false;
		}
		break;
	}
	/*case GuiControlType::SLIDER:
	{
		if (control->id == 1) app->audio->ChangeMusicVolume(sliderMusicVolume->ReturnValue());
		else if (control->id == 2) app->audio->ChangeFxVolume(sliderFxVolume->ReturnValue());
		break;
	}*/
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 1)
		{
			app->win->fullScreen = !app->win->fullScreen;
			app->win->ChangeScreenSize();
		}
		//else if (control->id == 2) app->vSync = !app->vSync;
		break;
	}
	default: break;
	}

	return true;
}

bool SceneIntro::CleanUp()
{
	app->tex->UnLoad(introText);
	app->tex->UnLoad(logoText);
	app->font->UnLoad(whiteFont);

	//RELEASE(btnStart);
	//RELEASE(btnContinue);
	//RELEASE(btnOptions);
	//RELEASE(btnExit);
	//RELEASE(btnBackOptions);
	//RELEASE(sliderMusicVolume);
	//RELEASE(sliderFxVolume);
	//RELEASE(checkBoxFullscreen);
	//RELEASE(checkBoxVSync);

	return true;
}