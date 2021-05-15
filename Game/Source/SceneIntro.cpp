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
#include "SceneDungeon.h"
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
	logoBackText = app->tex->Load("Assets/Textures/logoBack.png");
	logoRightText = app->tex->Load("Assets/Textures/logoRight.png");
	logoMidText = app->tex->Load("Assets/Textures/logoMid.png");
	logoLeftText = app->tex->Load("Assets/Textures/logoLeft.png");
	selectorTex = app->tex->Load("Assets/Textures/pointer.png");

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
	
	pos = 0;
	app->input->Enable();
	return ret;
}

bool SceneIntro::Update(float dt)
{
	GamePad& pad = app->input->pads[0];
	//Debug key for gamepad rumble testing purposes

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		app->input->ShakeController(0, 12, 0.33f);
	}

	//Debug key for gamepad rumble testing purposes
	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		app->input->ShakeController(0, 36, 0.66f);
	}

	//Debug key for gamepad rumble testing purposes
	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		app->input->ShakeController(0, 60, 1.0f);
	}

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
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.a == true) { Select(); }
		if (!options)
		{
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || pad.up == true)
			{
				pos--;
				if (pos < 0) pos = 3;
				if (pos == 1 && !posContinue) pos--;
			}

			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || pad.down == true)
			{
				pos++;
				if (pos > 3) pos = 0;
				if (pos == 1 && !posContinue) pos++;
			}
		}
		else if (options)
		{
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || pad.up == true)
			{
				pos--;
				if (pos < 4) pos = 5;
			}

			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || pad.down == true)
			{
				pos++;
				if (pos > 5) pos = 4;
			}
		}
	}
	return true;
}

// Update: draw background
bool SceneIntro::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	app->render->DrawTexture(logoBackText, 0, 555, NULL, 3);
	if (count > 0 && count <= 200) app->render->DrawTexture(logoMidText, 450, 555, NULL, 3);

	if (count > 5) app->render->DrawTexture(logoRightText, 1200, 555, NULL, 3);
	if (count > 10) app->render->DrawTexture(logoRightText, 1175, 555, NULL, 3);
	if (count > 15) app->render->DrawTexture(logoRightText, 1150, 555, NULL, 3);
	if (count > 20) app->render->DrawTexture(logoRightText, 1125, 555, NULL, 3);
	if (count > 25) app->render->DrawTexture(logoRightText, 1100, 555, NULL, 3);
	if (count > 30) app->render->DrawTexture(logoRightText, 1075, 555, NULL, 3);
	if (count > 35) app->render->DrawTexture(logoRightText, 1050, 555, NULL, 3);
	if (count > 40) app->render->DrawTexture(logoRightText, 1025, 555, NULL, 3);
	if (count > 45) app->render->DrawTexture(logoRightText, 1000, 555, NULL, 3);
	if (count > 50) app->render->DrawTexture(logoRightText, 975, 555, NULL, 3);
	if (count > 55) app->render->DrawTexture(logoRightText, 950, 555, NULL, 3);
	if (count > 60) app->render->DrawTexture(logoRightText, 925, 555, NULL, 3);
	if (count > 65) app->render->DrawTexture(logoRightText, 900, 555, NULL, 3);
	if (count > 70) app->render->DrawTexture(logoRightText, 875, 555, NULL, 3);
	if (count > 75 && count <= 200) app->render->DrawTexture(logoRightText, 850, 555, NULL, 3);


	if (count > 5) app->render->DrawTexture(logoLeftText, -350, 555, NULL, 3);
	if (count > 10) app->render->DrawTexture(logoLeftText, -325, 555, NULL, 3);
	if (count > 15) app->render->DrawTexture(logoLeftText, -300, 555, NULL, 3);
	if (count > 20) app->render->DrawTexture(logoLeftText, -275, 555, NULL, 3);
	if (count > 25) app->render->DrawTexture(logoLeftText, -250, 555, NULL, 3);
	if (count > 30) app->render->DrawTexture(logoLeftText, -225, 555, NULL, 3);
	if (count > 35) app->render->DrawTexture(logoLeftText, -200, 555, NULL, 3);
	if (count > 40) app->render->DrawTexture(logoLeftText, -175, 555, NULL, 3);
	if (count > 45) app->render->DrawTexture(logoLeftText, -150, 555, NULL, 3);
	if (count > 50) app->render->DrawTexture(logoLeftText, -125, 555, NULL, 3);
	if (count > 55) app->render->DrawTexture(logoLeftText, -100, 555, NULL, 3);
	if (count > 60) app->render->DrawTexture(logoLeftText, -75, 555, NULL, 3);
	if (count > 65) app->render->DrawTexture(logoLeftText, -50, 555, NULL, 3);
	if (count > 70) app->render->DrawTexture(logoLeftText, -25, 555, NULL, 3);
	if (count > 75 && count <= 200) app->render->DrawTexture(logoLeftText, 0, 555, NULL, 3);

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


		if (pos == 0)
		{
			posScaleX = btnStart->bounds.x - 20;
			posScaleY = btnStart->bounds.y - 2;
		}
		if (pos == 1 && posContinue)
		{
			posScaleX = btnContinue->bounds.x - 20;
			posScaleY = btnContinue->bounds.y - 2;
		}
		if (pos == 2)
		{
			posScaleX = btnOptions->bounds.x - 20;
			posScaleY = btnOptions->bounds.y - 2;
		}
		if (pos == 3)
		{
			posScaleX = btnExit->bounds.x - 20;
			posScaleY = btnExit->bounds.y - 2;
		}
		if (pos == 4)
		{
			posScaleX = btnBackOptions->bounds.x - 20;
			posScaleY = btnBackOptions->bounds.y - 2;
		}
		if (pos == 5)
		{
			posScaleX = checkBoxFullscreen->bounds.x - 125;
			posScaleY = checkBoxFullscreen->bounds.y + 1;
		}


		app->render->DrawTexture(selectorTex, posScaleX, posScaleY, NULL);
	}
	return ret;
}

void SceneIntro::Select()
{
	if (pos == 0)
	{
		app->fadeToBlack->FadeToBlk(this, app->scene, 30);
		startClicked = true;
		app->sceneBattle->enemy1Dead = false;
		app->sceneBattle->enemy2Dead = false;
		app->sceneBattle->enemy3Dead = false;
		app->sceneBattle->enemiesAlive = 3;
	}
	else if (pos == 1) app->fadeToBlack->FadeToBlk(this, app->scene, 30);

	else if (pos == 2)
	{
		options = true;
		pos = 5;
	}
	else if (pos == 3)
	{
		if (app->scene->player != nullptr)
		{
			app->scene->player->position.x = 350;
			app->scene->player->position.y = 875;
			app->SaveGameRequest();
		}
		exit = true;
	}
	else if (pos == 4)
	{
		btnOptions->state = GuiControlState::NORMAL;
		options = false;
		pos = 0;
	}
	else if (pos == 5)
	{
		app->win->fullScreen = !app->win->fullScreen;
		app->win->ChangeScreenSize();
	}

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
	app->tex->UnLoad(logoBackText);
	app->tex->UnLoad(logoMidText);
	app->tex->UnLoad(logoRightText);
	app->tex->UnLoad(logoLeftText);
	app->tex->UnLoad(selectorTex);
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