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
	return true;
}

bool SceneIntro::Start()
{
	LOG("Loading background assets");
	bool ret = true;

	introText = app->tex->Load("Assets/Textures/scene_intro.png");
	creditText = app->tex->Load("Assets/Textures/credits.png");
	logoText = app->tex->Load("Assets/Textures/logo.png");
	app->sceneLose->Disable();
	app->sceneWin->Disable();
	app->audio->PlayMusic("Assets/Audio/Music/intro_theme.ogg");
	app->render->camera.x = 0;
	app->render->camera.y = -555;

	char lookupTable[] = { "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ:/,!+-%  " };
	whiteFont = app->font->Load("Assets/Textures/white_font.png", lookupTable, 9);
	yellowFont = app->font->Load("Assets/Textures/yellow_font.png", lookupTable, 9);
	btnPlay = new GuiButton(1, { 180, 315, 48, 16 }, "PLAY");
	btnPlay->SetObserver(this);
	
	btnContinue = new GuiButton(2, { 160, 341, 90, 16 }, "CONTINUE");
	btnContinue->SetObserver(this);
	
	btnSettings = new GuiButton(3, { 329, 400, 90, 16 }, "SETTINGS");
	btnSettings->SetObserver(this);
	
	btnCredits = new GuiButton(4, { 19, 400, 80, 16 }, "CREDITS");
	btnCredits->SetObserver(this);
	
	btnExit = new GuiButton(5, { 371, 194, 48, 16 }, "EXIT");
	btnExit->SetObserver(this);
	
	btnBackSettings = new GuiButton(6, { 180, 400, 48, 16 }, "BACK");
	btnBackSettings->SetObserver(this);
	
	btnBackCredits = new GuiButton(7, { 180, 400, 48, 16 }, "BACK");
	btnBackCredits->SetObserver(this);

	btnControls = new GuiButton(8, { 160, 400, 90, 16 }, "CONTROLS");
	btnControls->SetObserver(this);

	btnBackControls = new GuiButton(9, { 180, 400, 48, 16 }, "BACK");
	btnBackControls->SetObserver(this);

	sliderMusicVolume = new GuiSlider(1, { 725, 950, 10, 28 }, "MUSIC VOLUME");
	sliderMusicVolume->SetObserver(this);
	
	sliderFxVolume = new GuiSlider(2, { 725, 1000, 10, 28 }, " FX VOLUME");
	sliderFxVolume->SetObserver(this);

	checkBoxFullscreen = new GuiCheckBox(1, { 675, 1050, 40, 40 }, "FULLSCREEN");
	checkBoxFullscreen->SetObserver(this);

	checkBoxVSync = new GuiCheckBox(2, { 675,1105,40,40 }, "   VSYNC");
	checkBoxVSync->SetObserver(this);
	return ret;
}

bool SceneIntro::Update(float dt)
{
	if (count <= 240) 
	{
		count++;
	}
	else if (count > 240)
	{
		if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) guiColliders = !guiColliders;

		if (settings == true)
		{
			sliderMusicVolume->Update(dt);
			sliderFxVolume->Update(dt);
			btnExit->Update(dt);
			btnBackSettings->Update(dt);
			btnSettings->Update(dt);
			btnCredits->Update(dt);
			checkBoxFullscreen->Update(dt);
			checkBoxVSync->Update(dt);
		}
		else if (credits == true)
		{
			btnExit->Update(dt);
			btnBackCredits->Update(dt);
			btnSettings->Update(dt);
			btnCredits->Update(dt);
		}

		else if (controls == true)
		{
			btnExit->Update(dt);
			btnBackControls->Update(dt);
			btnSettings->Update(dt);
			btnCredits->Update(dt);
		}
		else
		{
			btnPlay->Update(dt);
			btnContinue->Update(dt);
			btnSettings->Update(dt);
			btnCredits->Update(dt);
			btnExit->Update(dt);
			btnControls->Update(dt);
		}
	}
	return true;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	if (count <= 240) app->render->DrawTexture(logoText, 0, 185, NULL);

	else if (count > 240)
	{
		app->render->DrawTexture(introText, 0, 185, NULL);

		if (settings == true)
		{
			app->render->DrawTexture(creditText, 220, 900, NULL);
			sliderMusicVolume->Draw();
			sliderFxVolume->Draw();
			btnBackSettings->Draw();
			btnExit->Draw();
			btnCredits->Draw();
			btnSettings->Draw();
			checkBoxFullscreen->Draw();
			checkBoxVSync->Draw();
		}
		else if (credits == true)
		{
			app->render->DrawTexture(creditText, 220, 900, NULL);
			app->font->DrawText(450, 380, yellowFont, "FAKE XEICS");
			app->font->DrawText(420, 450, whiteFont, "ARNAU BONADA");
			app->font->DrawText(430, 500, whiteFont, "MAX LLOVERA");
			app->font->DrawText(405, 550, whiteFont, "ARNAU USTRELL");
			btnBackCredits->Draw();
			btnExit->Draw();
			btnCredits->Draw();
			btnSettings->Draw();
		}
		else if (controls == true)
		{
			app->render->DrawTexture(creditText, 220, 900, NULL);
			app->font->DrawText(300, 380, whiteFont, "MOVE LEFT");
			app->font->DrawText(300, 425, whiteFont, "MOVE RIGHT");
			app->font->DrawText(300, 470, whiteFont, "JUMP");
			app->font->DrawText(300, 515, whiteFont, "SHOOT");
			app->font->DrawText(300, 560, whiteFont, "USE OBJECT");
			app->font->DrawText(700, 380, yellowFont, "A");
			app->font->DrawText(700, 425, yellowFont, "D");
			app->font->DrawText(700, 470, yellowFont, "SPACE");
			app->font->DrawText(700, 515, yellowFont, "P");
			app->font->DrawText(700, 560, yellowFont, "E");
			btnBackControls->Draw();
			btnExit->Draw();
			btnCredits->Draw();
			btnSettings->Draw();
		}
		else
		{
			btnPlay->Draw();
			btnContinue->Draw();
			if (!posContinue || app->sceneWin->won || app->sceneLose->lost) btnContinue->state = GuiControlState::DISABLED;
			else if (posContinue) btnContinue->state = GuiControlState::NORMAL;
			btnSettings->Draw();
			btnCredits->Draw();
			btnExit->Draw();
			btnControls->Draw();
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
			playClicked = true;
		}
		else if (control->id == 2) app->fadeToBlack->FadeToBlk(this, app->scene, 30);

		else if (control->id == 3)
		{
			btnCredits->state = GuiControlState::DISABLED;
			btnSettings->state = GuiControlState::DISABLED;
			settings = true;
		}
		else if (control->id == 4)
		{
			btnCredits->state = GuiControlState::DISABLED;
			btnSettings->state = GuiControlState::DISABLED;
			credits = true;
		}
		else if (control->id == 5)
		{
			if (app->scene->player != nullptr)
			{
				app->scene->player->position.x = 350;
				app->scene->player->position.y = 875;
				app->SaveGameRequest();
			}
			exit = true;
		}
		else if (control->id == 6)
		{
			btnCredits->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			settings = false;
		}
		else if (control->id == 7)
		{
			btnCredits->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			credits = false;
		}
		else if (control->id == 8)
		{
			btnCredits->state = GuiControlState::DISABLED;
			btnSettings->state = GuiControlState::DISABLED;
			controls = true;
		}
		
		else if (control->id == 9)
		{
			btnCredits->state = GuiControlState::NORMAL;
			btnSettings->state = GuiControlState::NORMAL;
			controls = false;
		}
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 1) app->audio->ChangeMusicVolume(sliderMusicVolume->ReturnValue());
		else if (control->id == 2) app->audio->ChangeFxVolume(sliderFxVolume->ReturnValue());
		break;
	}
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 1)
		{
			app->win->fullScreen = !app->win->fullScreen;
			app->win->ChangeScreenSize();
		}
		else if (control->id == 2) app->vSync = !app->vSync;
		break;
	}
	default: break;
	}

	return true;
}

bool SceneIntro::CleanUp()
{
	app->tex->UnLoad(introText);
	app->tex->UnLoad(creditText);
	app->tex->UnLoad(logoText);
	app->font->UnLoad(whiteFont);
	app->font->UnLoad(yellowFont);
	return true;
}