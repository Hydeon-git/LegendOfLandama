#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Font.h"
#include "FlyingEnemy.h"
#include "EntityManager.h"
#include "FadeToBlack.h"
#include "PathFinding.h"
#include "SceneIntro.h"
#include "SceneLose.h"
#include "SceneWin.h"
#include "ModuleParticles.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

#include "Defs.h"
#include "Log.h"



Scene::Scene() : Module()
{
	name.Create("scene");

	puzzleRect = SDL_Rect{ 0,74,40,43 };

	clockAnim.PushBack({ 0,0,40,34 });
	clockAnim.PushBack({ 0,34,40,34 });
	clockAnim.speed = 0.0168f;
	
	lifesAnim.PushBack({ 0,0,54,56 });
	lifesAnim.PushBack({ 0,56,54,56 });
	lifesAnim.speed = 0.02f;

	keyAnim.PushBack({ 0,0,32,34 });
	keyAnim.PushBack({ 0,34,32,34 });
	keyAnim.speed = 0.04f;

	puzzleAnim.PushBack({ 0,0,40,37 });
	puzzleAnim.PushBack({ 0,37,40,37 });
	puzzleAnim.speed = 0.04f;
}

// Destructor
Scene::~Scene(){}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L12b: Create walkability map on map loading
	if (this->active == true)
	{
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
		flyingEnemy = (FlyingEnemy*)app->entityManager->CreateEntity(EntityType::FLYING_ENEMY);
		particles = (ModuleParticles*)app->entityManager->CreateEntity(EntityType::PARTICLE);

		player->Start();
		//player->active = true;
		enemy->Start();
		flyingEnemy->Start();
		particles->Start();

		app->map->Enable();
		background = app->tex->Load("Assets/Textures/background.png");
		heart = app->tex->Load("Assets/Textures/head_life.png");
		key = app->tex->Load("Assets/Textures/key.png");
		puzzle = app->tex->Load("Assets/Textures/puzzle.png");
		pause = app->tex->Load("Assets/Textures/pause.png");
		creditText = app->tex->Load("Assets/Textures/settings_paused.png");
		clockText = app->tex->Load("Assets/Textures/timer.png");
		player->spiked = false;

		char lookupTable[] = { "! #$%& ()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~"};
		whiteFont = app->font->Load("Assets/Textures/white_font.png", lookupTable, 1);


		if(!app->sceneIntro->posContinue) timer = 0;

		// L03: DONE: Load map

		if (app->map->Load("town.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}

		// Load music
		app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	}



	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	//PROVISIONAL CAMERA
	app->render->camera.x = -player->position.x*2;
	app->render->camera.y = -player->position.y*2;

	// L02: DONE 3: Request Load / Save when pressing L/S
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && !paused && !pausedSettings) app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();


	//restart from first level
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && !paused && !pausedSettings)
	{
		restart = true;
		app->render->RestartValues();
	}

	//restart the current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && !paused && !pausedSettings)
	{
		restart = true;
		app->render->RestartValues();
	}

	//view colliders
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) app->map->colliders = !app->map->colliders;

	//god mode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) player->godModeEnabled = !player->godModeEnabled;

	//cap fps
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) app->capped = !app->capped;

	//SceneWin
	if (player->win)
	{
		app->sceneWin->won = true;
		player->win = true;
		app->fadeToBlack->FadeToBlk(this, app->sceneWin, 30);

		app->render->RestartValues();
	}

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && !paused && !pausedSettings)
	{
		if (player->position.x <= 938 && app->scene->player->position.x >= 851 && app->scene->player->position.y <= 171 && app->scene->player->position.y >= 81)
		{
			app->scene->player->position.x = 350;
			app->scene->player->position.y = 875;
			app->render->camera.x = 0;
			app->render->camera.y = -555;
		}
		else
		{
			player->position.x = 938;
			player->position.y = 171;
			app->render->camera.x = -588;
			app->render->camera.y = -99;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) guiColliders = !guiColliders;

	if (!app->scene->paused)
	{
		//camera x
		if ((app->render->counter == 0 || player->godModeEnabled) && !player->spiked && !paused)
		{
			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && player->position.x > 350 && player->position.x <= 4400 && !player->ThereIsLeftWall() && !player->ThereIsChestLeft()) app->render->camera.x += 3.0f;
			else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && player->position.x >= 350 && player->position.x < 4400 && !player->ThereIsRightWall() && !player->ThereIsChestRight() && !player->ThereIsLeftWall()) app->render->camera.x -= 3.0f;
		}
		//camera y

		if (player->position.y < 570 && app->render->camera.y < -100) app->render->camera.y += 4.0f;

		if (player->position.y >= 570 && app->render->camera.y > -550) app->render->camera.y -= 4.0f;
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (!pausedSettings)
		{
			paused = true;
			Pause();
		}
		
	}

	if (paused && pausedSettings)
	{
		sliderMusicVolume->Update(dt);
		sliderFxVolume->Update(dt);
		checkBoxFullscreen->Update(dt);
		checkBoxVSync->Update(dt);
		btnBack->Update(dt);
	}
	else if (!pausedSettings && paused)
	{
		btnResume->Update(dt);
		btnSettings->Update(dt);
		btnBackIntro->Update(dt);
		btnExit->Update(dt);
	}

	if (!paused) clockAnim.Update();
	if (!paused) lifesAnim.Update();
	if (!paused) keyAnim.Update();
	if (!paused) puzzleAnim.Update();

	lifesScene = player->lifes;
	sceneCounterKey = player->counterKey;
	sceneCounterCheckpoint = player->counterCheckpoint;
	sceneCounterHeart = player->counterHeart;
	sceneCounterPuzzle = player->counterPuzzle;

	if (app->sceneIntro->exit == true) return false;
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;


	// Draw map
	//app->render->DrawTexture(background, 0, 0);
	app->map->Draw();
	app->map->DrawVillage();
	app->map->DrawColliders();
	app->map->DrawCheckpoint();
	app->map->DrawPuzzle();
	if (!app->map->chestTaken) app->map->DrawChest();
	if (app->map->chestTaken) app->map->DrawHeart();

	if(app->map->keyTaken) app->render->DrawTexture(key, -app->render->camera.x + 10, -app->render->camera.y + 75, &(keyAnim.GetCurrentFrame()));

	if(app->map->puzzleTaken&&!app->map->chestTaken) app->render->DrawTexture(puzzle, -app->render->camera.x + 70, -app->render->camera.y + 71, &(puzzleAnim.GetCurrentFrame()));
	
	if (app->map->chestTaken) app->render->DrawTexture(puzzle, -app->render->camera.x + 70, -app->render->camera.y + 71, &puzzleRect);


	//menu pause

	if (pausedSettings)
	{
		app->render->DrawTexture(creditText, -app->render->camera.x + 220, -app->render->camera.y + 150, NULL);
		sliderMusicVolume->Draw();
		sliderFxVolume->Draw();
		checkBoxFullscreen->Draw();
		checkBoxVSync->Draw();
		btnBack->Draw();
	}
	else if (paused)
	{
		app->render->DrawTexture(pause, -app->render->camera.x + 391, -app->render->camera.y + 100);
		btnResume->Draw();
		btnSettings->Draw();
		btnBackIntro->Draw();
		btnExit->Draw();
	}

	if (!paused)
	{
		seconds++;
		if (seconds == 60) 
		{
			timer++;
			seconds = 0;
		}
	}
	sprintf_s(timerText, 10, "%d", timer);
	if (timer < 10) app->font->DrawText(1236, 10, whiteFont, timerText);
	else if (timer < 100) app->font->DrawText(1206, 10, whiteFont, timerText);
	else if (timer < 1000) app->font->DrawText(1174, 10, whiteFont, timerText);
	else app->font->DrawText(1145, 10, whiteFont, timerText);

	
	app->render->DrawTexture(clockText, -app->render->camera.x+1100, -app->render->camera.y+10, &(clockAnim.GetCurrentFrame()));

	if (guiColliders)
	{
		app->render->DrawRectangle({ -app->render->camera.x + 1100, -app->render->camera.y+10 ,40,35 }, 0, 0, 100, 100);

		if (timer < 10) app->render->DrawRectangle({ -app->render->camera.x + 1235, -app->render->camera.y + 10 ,35,35 }, 200, 100, 0, 100);
		else if (timer < 100) app->render->DrawRectangle({ -app->render->camera.x + 1204, -app->render->camera.y + 10 ,70,35 }, 200, 100, 0, 100);
		else if (timer < 1000) app->render->DrawRectangle({ -app->render->camera.x + 1170, -app->render->camera.y + 10 ,105,35 }, 200, 100, 0, 100);
		else app->render->DrawRectangle({ -app->render->camera.x + 1137, -app->render->camera.y + 10 ,130,35 }, 200, 100, 0, 100);

		if (player->lifes == 4) app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y ,246,56 }, 0, 200, 0, 100);
		if (player->lifes == 3) app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y ,182,56 }, 0, 200, 0, 100);
		if (player->lifes == 2) app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y ,118,56 }, 0, 200, 0, 100);
		if (player->lifes == 1) app->render->DrawRectangle({ -app->render->camera.x, -app->render->camera.y ,54,56 }, 0, 200, 0, 100);

		if (app->map->keyTaken) app->render->DrawRectangle({ -app->render->camera.x + 10, -app->render->camera.y + 75,32,34 }, 255, 0, 255, 100);

		if (app->map->puzzleTaken && !app->map->chestTaken) app->render->DrawRectangle({ -app->render->camera.x + 70, -app->render->camera.y + 71,40,37 }, 200, 200, 0, 100);
		if (app->map->chestTaken) app->render->DrawRectangle({ -app->render->camera.x + 70, -app->render->camera.y + 71,40,43 }, 200, 100, 200, 100);
	}
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(background);
	app->tex->UnLoad(heart);
	app->tex->UnLoad(key);
	app->tex->UnLoad(puzzle);
	app->tex->UnLoad(pause);
	app->tex->UnLoad(creditText);
	app->tex->UnLoad(clockText);
	app->font->UnLoad(whiteFont);
	app->entityManager->DestroyEntity(player);
	app->entityManager->DestroyEntity(enemy);
	app->entityManager->DestroyEntity(flyingEnemy);
	app->entityManager->DestroyEntity(particles);

	return true;
}



bool Scene::LoadState(pugi::xml_node& node)
{
	bool ret = true;
	app->scene->player->position.x = node.child("positionPlayer").attribute("x").as_int();
	app->scene->player->position.y = node.child("positionPlayer").attribute("y").as_int();
	return ret;
}

bool Scene::SaveState(pugi::xml_node& node) const
{
	bool ret = true;
	pugi::xml_node pnode = node.append_child("positionPlayer");
	pnode.append_attribute("x") = app->scene->player->position.x;
	pnode.append_attribute("y") = app->scene->player->position.y;
	return ret;
}

void Scene::Pause()
{
	app->tex->UnLoad(app->scene->player->texPlayer);
	app->tex->UnLoad(app->scene->enemy->texEnemy);
	app->tex->UnLoad(app->scene->flyingEnemy->texFlyingEnemy);
	app->tex->UnLoad(app->scene->particles->texture);


	//SDL_Rect rect = { -app->render->camera.x + 450, -app->render->camera.y + 50, 400, 450 };

	//Buttons
	btnResume = new GuiButton(1, { -app->render->camera.x+535, -app->render->camera.y+160, 210, 50 }, "Resume");
	btnResume->SetObserver(this);

	btnSettings = new GuiButton(2, { -app->render->camera.x+505, -app->render->camera.y+263, 269, 51 }, "SETTINGS");
	btnSettings->SetObserver(this);

	btnBackIntro = new GuiButton(3, { -app->render->camera.x+485, -app->render->camera.y+368, 310, 50 }, "BACK MENU");
	btnBackIntro->SetObserver(this);

	btnExit = new GuiButton(4, { -app->render->camera.x+569, -app->render->camera.y+471, 145, 50 }, "EXIT");
	btnExit->SetObserver(this);

	btnBack = new GuiButton(5, { -app->render->camera.x + 865, -app->render->camera.y + 510,145 ,50 }, "BACK");
	btnBack->SetObserver(this);

	sliderMusicVolume = new GuiSlider(1, { -app->render->camera.x + 725, -app->render->camera.y + 220, 10, 28 }, "MUSIC VOLUME");
	sliderMusicVolume->SetObserver(this);

	sliderFxVolume = new GuiSlider(2, { -app->render->camera.x + 725, -app->render->camera.y + 300, 10, 28 }, " FX VOLUME");
	sliderFxVolume->SetObserver(this);

	checkBoxFullscreen = new GuiCheckBox(1, { -app->render->camera.x + 675, -app->render->camera.y + 380, 40, 40 }, "FULLSCREEN");
	checkBoxFullscreen->SetObserver(this);

	checkBoxVSync = new GuiCheckBox(2, { -app->render->camera.x + 675, -app->render->camera.y + 460,40,40 }, "   VSYNC");
	checkBoxVSync->SetObserver(this);



}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1)
		{
			paused = false;
			app->scene->player->texPlayer= app->tex->Load("Assets/Textures/player_textures.png");
			app->scene->enemy->texEnemy = app->tex->Load("Assets/Textures/enemy_texture.png");
			app->scene->flyingEnemy->texFlyingEnemy = app->tex->Load("Assets/Textures/flying_enemy_texture.png");
			app->scene->particles->texture = app->tex->Load("Assets/Textures/shot_fireball.png");
		}
		else if (control->id == 2) pausedSettings = true;
		else if (control->id == 3)
		{
			app->fadeToBlack->FadeToBlk(this, app->sceneIntro, 30);
			paused = false;
		}
		else if (control->id == 4)
		{
			if (app->scene->player != nullptr)
			{
				app->scene->player->position.x = 350;
				app->scene->player->position.y = 875;
				app->SaveGameRequest();
			}
			app->sceneIntro->exit = true;
		}
		else if (control->id == 5) pausedSettings = false;
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