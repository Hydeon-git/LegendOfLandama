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
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
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
#include "HUD.h"


SceneDungeon::SceneDungeon() : Module()
{
	name.Create("SceneDungeon");
}

SceneDungeon::~SceneDungeon()
{

}

bool SceneDungeon::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	scale = (int)app->win->GetScale();
	fullscreenRect = new SDL_Rect{ 0, 0, 1280, 720 };
	bool ret = true;
	return ret;
}

bool SceneDungeon::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		currentScene = DungeonScene::SCENE_HALL;
		LOG("Loading Scene Dungeon");
		// Font
		winText = app->tex->Load("Assets/Textures/scene_win.png");
		char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
		whiteFont = app->font->Load("Assets/Textures/white_font_mini.png", lookupTable, 1);

		// Map
		app->map->Enable();

		// Music
		app->audio->PlayMusic("Assets/Audio/Music/dungeon_music.ogg");

		// Entities
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);

		npc5 = (NPC5*)app->entityManager->CreateEntity(EntityType::NPC5);
		npc7 = (NPC7*)app->entityManager->CreateEntity(EntityType::NPC7);

		enemy1 = (Enemy1*)app->entityManager->CreateEntity(EntityType::Enemy1);
		enemy2 = (Enemy2*)app->entityManager->CreateEntity(EntityType::Enemy2);
		enemy3 = (Enemy3*)app->entityManager->CreateEntity(EntityType::Enemy3);

		app->scene->player->dead = false;
		player->Start();

		npc5->Start();
		npc7->Start();

		enemy1->Start();
		enemy2->Start();
		enemy3->Start();		

		// Texture assignations for the scene dungeon
		texMenu = app->tex->Load("Assets/Textures/pause_menu.png");


		// Camera
		app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
		app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;

		// Map Load
		if (app->map->Load("dungeonHall.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}

		//Buttons
		btnResume = new GuiButton(1, { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 115, 70, 12 }, "RESUME");
		btnResume->SetObserver(this);

		btnSettings = new GuiButton(2, { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 135, 70, 12 }, "SETTINGS");
		btnSettings->SetObserver(this);

		btnBackIntro = new GuiButton(3, { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 155, 70, 12 }, "BACK MENU");
		btnBackIntro->SetObserver(this);

		btnExit = new GuiButton(4, { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 175, 70, 12 }, "EXIT");
		btnExit->SetObserver(this);

		btnBack = new GuiButton(5, { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 200,70 ,12 }, "BACK");
		btnBack->SetObserver(this);

		checkBoxFullscreen = new GuiCheckBox(1, { -app->render->camera.x / 3, -app->render->camera.y / 3, 40, 40 }, "FULLSCREEN");
		checkBoxFullscreen->SetObserver(this);
	}
	return ret;
}

bool SceneDungeon::Update(float dt)
{
	if (currentScene == DungeonScene::SCENE_HALL && player->door == COLLIDER_GREEN_DUNGEON)
	{
		ChangeScene(DungeonScene::SCENE_MID);
		//app->audio->PlayFx(doorCloseFx, 0);
		player->door = 0;
	}
	else if (currentScene == DungeonScene::SCENE_MID && player->door == COLLIDER_GREEN_DUNGEON)
	{
		ChangeScene(DungeonScene::SCENE_BOSS);
		//app->audio->PlayFx(doorCloseFx, 0);
		player->door = 0;
	}

	if (!paused && !app->hud->bagIsOpen)
	{
		// Camera x
		if ((app->render->counter == 0 || player->godModeEnabled))
		{
			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
				&& !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
				&& !(app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
				&& !(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				&& !player->ThereIsLeftWall() 
				&& !player->ThereIsNPCLeft())
			{
				app->render->camera.x += 3.0f;
			}
				

			else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
				&& !(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				&& !player->ThereIsRightWall()
				&& !player->ThereIsNPCRight())
			{
				app->render->camera.x -= 3.0f;
			}
				
		}
		// Camera y
		if ((app->render->counter == 0 || player->godModeEnabled))
		{

			if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
				&& !player->ThereIsTopWall()
				&& !player->ThereIsNPCUp()
				&& !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
			{
				app->render->camera.y += 3.0f;
			}
			else if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
				&& !player->ThereIsBottomWall()
				&& !player->ThereIsNPCBelow()
				&& !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
			{
				app->render->camera.y -= 3.0f;
			}
		}
	}

	//God Mode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		// Print colliders manually implemented
		app->map->colliders = !app->map->colliders;

		// Print colliders from Collision Modulet
		app->debug = !app->debug;

		player->godModeEnabled = !player->godModeEnabled;
	}

	// Pause Menu
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && !app->scene->player->dialogeOn)
	{
		if (!pausedSettings)
		{
			paused = true;
			Pause();
		}
	}

	// Pause Menu
	if (pausedSettings)
	{
		//sliderMusicVolume->Update(dt);
		//sliderFxVolume->Update(dt);
		checkBoxFullscreen->Update(dt);
		//checkBoxVSync->Update(dt);
		btnBack->Update(dt);
	}
	else if (!pausedSettings && paused)
	{
		btnResume->Update(dt);
		btnSettings->Update(dt);
		btnBackIntro->Update(dt);
		btnExit->Update(dt);
	}

	return true;
}

// Update: draw background
bool SceneDungeon::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	app->map->Draw();

	app->map->DrawDoor();
	app->map->DrawDoorMid();
	app->map->DrawChest();
	app->map->DrawLavers();
	if (!app->map->puzzle1DungeonDone)
	{
		app->map->DrawWalls2Dungeon();
	}
	if (app->map->buttonFloorPressed)
	{
		app->map->DrawFloor2Dungeon();
	}
	
	//spikes
	if (!app->map->laver1Pressed && !app->map->laver2Pressed && !app->map->laver3Pressed && !app->map->laverFinalPressed)
	{
		app->map->DrawSpikes1();
		app->map->DrawSpikes2();
		app->map->DrawSpikes3();
	}
	else if (app->map->laver1Pressed && !app->map->laver2Pressed && !app->map->laver3Pressed && !app->map->laverFinalPressed)
	{
		//app->map->DrawSpikes1();
		app->map->DrawSpikes2();
		app->map->DrawSpikes3();
	}
	else if (app->map->laver1Pressed && app->map->laver2Pressed && !app->map->laver3Pressed && !app->map->laverFinalPressed)
	{
		app->map->DrawSpikes1();
		//app->map->DrawSpikes2();
		app->map->DrawSpikes3();
	}
	else if (app->map->laver1Pressed && app->map->laver2Pressed && app->map->laver3Pressed && !app->map->laverFinalPressed)
	{
		//app->map->DrawSpikes1();
		//app->map->DrawSpikes2();
		app->map->DrawSpikes3();
	}
	//else if ((!app->map->laver1Pressed && !app->map->laver2Pressed && !app->map->laver3Pressed && app->map->laverFinalPressed) || (app->map->laver1Pressed && !app->map->laver2Pressed && !app->map->laver3Pressed && app->map->laverFinalPressed))
	//{
	//	app->map->DrawSpikes1();
	//	app->map->DrawSpikes2();
	//	app->map->DrawSpikes3();
	//}

	app->map->DrawColliders();

	// Pause Menu
	if (pausedSettings)
	{
		app->render->DrawTexture(texMenu, -app->render->camera.x, -app->render->camera.y, fullscreenRect, 3);
		//sliderMusicVolume->Draw();
		//sliderFxVolume->Draw();
		checkBoxFullscreen->Draw();
		//checkBoxVSync->Draw();
		btnBack->Draw();
	}
	else if (paused)
	{
		//app->render->DrawRectangle({ -app->render->camera.x/3 , -app->render->camera.y / 3  ,500,500 }, 0, 0, 0, 120);
		app->render->DrawTexture(texMenu, -app->render->camera.x, -app->render->camera.y, fullscreenRect, 3);
		btnResume->Draw();
		btnSettings->Draw();
		btnBackIntro->Draw();
		btnExit->Draw();
	}

	return ret;
}

void SceneDungeon::Pause()
{
	btnResume->bounds = { -app->render->camera.x / 3 + 170,-app->render->camera.y / 3 + 115, 70, 12 };
	btnSettings->bounds = { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 135, 70, 12 };
	btnBackIntro->bounds = { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 155, 70, 12 };
	btnExit->bounds = { -app->render->camera.x / 3 + 170,  -app->render->camera.y / 3 + 175, 70, 12 };
	btnBack->bounds = { -app->render->camera.x / 3 + 190,  -app->render->camera.y / 3 + 175, 70, 12 };
	checkBoxFullscreen->bounds = { -app->render->camera.x / 3 + 250,  -app->render->camera.y / 3 + 135, 15,15 };
}

bool SceneDungeon::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1)
		{
			paused = false;

		}
		else if (control->id == 2) pausedSettings = true;
		else if (control->id == 3)
		{
			app->map->CleanUp();
			//if (currentScene != GameScene::SCENE_TOWN)
			//{
			//	currentScene = GameScene::SCENE_TOWN;
			//}
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


void SceneDungeon::ChangeScene(DungeonScene nextScene)
{
	LOG("Changing scene");

	// Clearing Map
	app->map->CleanUp();

	switch (nextScene)
	{
	case DungeonScene::SCENE_NONE:
	{
		LOG("ERROR: Scene loaded was none so intro scene loaded instead.");
		ChangeScene(DungeonScene::SCENE_HALL);
		break;
	}
	case DungeonScene::SCENE_HALL:
	{
		if (app->map->Load("dungeonHall.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}

		currentScene = DungeonScene::SCENE_HALL;
	}
	break;

	case DungeonScene::SCENE_MID:
	{
		if (app->map->Load("dungeonMid.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}
		app->sceneDungeon->player->position.x = 72;
		app->sceneDungeon->player->position.y = 287;
		app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
		app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;
		currentScene = DungeonScene::SCENE_MID;
	} break;

	case DungeonScene::SCENE_BOSS:
	{
		if (app->map->Load("dungeonFinal.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}
		app->sceneDungeon->player->position.x = 248;
		app->sceneDungeon->player->position.y = 573;
		app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
		app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;
		currentScene = DungeonScene::SCENE_BOSS;
	} break;
	}
}

bool SceneDungeon::CleanUp()
{
	app->font->UnLoad(whiteFont);
	app->tex->UnLoad(winText);
	return true;
}