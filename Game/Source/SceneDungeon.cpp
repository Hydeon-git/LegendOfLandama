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
	name.Create("sceneDungeon");	
}

// Destructor
SceneDungeon::~SceneDungeon(){}

// Called before render is available
bool SceneDungeon::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneDungeon");
	scale = (int)app->win->GetScale();
	fullscreenRect = new SDL_Rect{ 0, 0, 1280, 720};
	bool ret = true;
	return ret;
}

// Called before the first frame
bool SceneDungeon::Start()
{
	// L12b: Create walkability map on map loading
	if (this->active == true)
	{
		// Set Current Scene to TOWN
		currentScene = DungeonScene::SCENE_HALL;

		// Setting dialogue to id 0 - Beach Girl
		
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);

		// Starts entities that are on the TOWN
	
		player->Start();
		// Enables the Map
		app->map->Enable();
		player->spiked = false;
		player->onBattle = false;
		
		// Texture assignations for the scene
		texMenu = app->tex->Load("Assets/Textures/pause_menu.png");
		doorOpenFx = app->audio->LoadFx("Assets/Audio/Fx/doorOpen.wav");
		doorCloseFx = app->audio->LoadFx("Assets/Audio/Fx/doorClose.wav");
		doorKnokFx = app->audio->LoadFx("Assets/Audio/Fx/doorKnok.wav");

		enemyMoving = false;
		stop = false;

		char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
		whiteFont = app->font->Load("Assets/Textures/white_font_mini.png", lookupTable, 1);

		app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
		app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;

		if(!app->sceneIntro->posContinue) timer = 0;

		// Loads the map and creates walkability map

		if (app->map->Load("dungeonHall.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}
		// Loads music
		app->audio->PlayMusic("Assets/Audio/Music/main_theme.ogg");

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

		/*sliderMusicVolume = new GuiSlider(1, { -app->render->camera.x / 3, -app->render->camera.y / 3 , 10, 28 }, "MUSIC VOLUME");
		sliderMusicVolume->SetObserver(this);

		sliderFxVolume = new GuiSlider(2, { -app->render->camera.x / 3, -app->render->camera.y / 3, 10, 28 }, " FX VOLUME");
		sliderFxVolume->SetObserver(this);*/

		checkBoxFullscreen = new GuiCheckBox(1, { -app->render->camera.x / 3, -app->render->camera.y / 3, 40, 40 }, "FULLSCREEN");
		checkBoxFullscreen->SetObserver(this);

		/*checkBoxVSync = new GuiCheckBox(2, { -app->render->camera.x / 3, -app->render->camera.y / 3,40,40 }, "   VSYNC");
		checkBoxVSync->SetObserver(this);*/
	}

	return true;
}

// Called each loop iteration
bool SceneDungeon::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneDungeon::Update(float dt)
{	
	//View Colliders
	//God Mode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		// Print colliders manually implemented
		app->map->colliders = !app->map->colliders;

		// Print colliders from Collision Modulet
		app->debug = !app->debug;

		player->godModeEnabled = !player->godModeEnabled;
	}

	// Request Load / Save when pressing F6/F5
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && !paused && !pausedSettings) app->LoadGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();

	// Pause Menu
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN&&!app->scene->player->dialogeOn)
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

	if (app->sceneIntro->exit == true) return false;
	// Current Scene Update()
		
	return true;
}

// Called each loop iteration
bool SceneDungeon::PostUpdate()
{
	bool ret = true;

	// Draw map
	app->map->Draw();
	app->map->DrawColliders();
	app->map->DrawCheckpoint();

	// Draw Road Signal
	if (player->ThereIsLimit())
	{
		app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
		app->render->DrawRectangle({ 10, 590, 1260, 120}, 100, 100, 200, 220, true, false);
		
		app->font->DrawText(90, 205, whiteFont, "This road is too dangerous.");
		app->font->DrawText(90, 220, whiteFont, "  Go back to the village.");
	}

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

// Called before quitting
bool SceneDungeon::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(pause);
	app->tex->UnLoad(creditText);
	app->font->UnLoad(whiteFont);
	app->entityManager->DestroyEntity(player);
	
	return true;
}

void SceneDungeon::ChangeScene(DungeonScene nextScene)
{
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

		npc1 = (NPC1*)app->entityManager->CreateEntity(EntityType::NPC1);
		npc1->Start();

		// Setting dialogue to id 0 Beach Girl and restart dialog system
		/*app->dialogueSystem->CleanUp();
		app->dialogueSystem->id = 0;
		app->dialogueSystem->Start();
		app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];*/

		// Deteting in which house the player was
		// Reposition the player
		switch (house)
		{
			// 0 - None
			// 1 - House1
			// 2 - Blacksmith
			// 3 - Inn
			// 4 - Dungeon Entry
		case 1:
		{
			app->scene->player->position.x = 144;
			app->scene->player->position.y = 99;
			player->doorTaked = true;

			if (player->doorTaked)
			{
				//player->posMoved = 0;
				player->doorTaked2 = true;
			}
			//player->doorTaked = true;
			//player->doorTakedY = true;
			player->lastPositionX2 = player->position.x;
			player->lastPositionY2 = player->position.y;

			app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
			app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;

			npc3->CleanUp();
			app->entityManager->DestroyEntity(npc3);
			npc3 = nullptr;

			house = 0;
		} break;
		case 2:
		{
			app->scene->player->position.x = 544;
			app->scene->player->position.y = 130;
			player->doorTaked = true;

			if (player->doorTaked)
			{
				//player->posMoved = 0;
				player->doorTaked2 = true;

			}
			//player->doorTaked = true;
			//player->doorTakedY = true;
			player->lastPositionX2 = player->position.x;
			player->lastPositionY2 = player->position.y;
			app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
			app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;

			npc2->CleanUp();
			app->entityManager->DestroyEntity(npc2);
			npc2 = nullptr;

			house = 0;
		} break;
		case 3:
		{
			app->scene->player->position.x = 401;
			app->scene->player->position.y = 320;
			player->doorTaked = true;

			if (player->doorTaked)
			{
				//player->posMoved = 0;
				player->doorTaked2 = true;

			}
			//player->doorTakedY = true;
			player->lastPositionX2 = player->position.x;
			player->lastPositionY2 = player->position.y;
			app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
			app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;

			npc4->CleanUp();
			app->entityManager->DestroyEntity(npc4);
			npc4 = nullptr;

			house = 0;
		} break;
		case 4:
		{
			app->scene->player->position.x = 401;
			app->scene->player->position.y = 320;
			player->doorTaked = true;

			if (player->doorTaked)
			{
				player->doorTaked2 = true;

			}
			player->lastPositionX2 = player->position.x;
			player->lastPositionY2 = player->position.y;
			app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
			app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;

			house = 0;
		}break;
		}
		currentScene = DungeonScene::SCENE_HALL;
	}
	break;
	case DungeonScene::SCENE_MID:
	{
		if (app->map->Load("house1.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}

		// Unload Beach Girl
		npc1->CleanUp();
		app->entityManager->DestroyEntity(npc1);
		npc1 = nullptr;

		// Creates Fisherman and starts it	
		npc3 = (NPC3*)app->entityManager->CreateEntity(EntityType::NPC3);
		npc3->Start();

		// Setting dialogue to id 2 Fisherman and restart dialog system

		/*app->dialogueSystem->CleanUp();
		app->dialogueSystem->id = 2;
		app->dialogueSystem->Start();
		app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];*/

		house = 1;
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		app->scene->player->position.x = 153;
		app->scene->player->position.y = 156;

		player->doorTaked = true;
		//player->doorTakedY = true;
		player->lastPositionX2 = player->position.x;
		player->lastPositionY2 = player->position.y;

		currentScene = DungeonScene::SCENE_MID;
	} break;

	}
}

bool SceneDungeon::LoadState(pugi::xml_node& node)
{
	bool ret = true;
	app->scene->player->position.x = node.child("positionPlayer").attribute("x").as_int();
	app->scene->player->position.y = node.child("positionPlayer").attribute("y").as_int();
	return ret;
}

bool SceneDungeon::SaveState(pugi::xml_node& node) const
{
	bool ret = true;
	pugi::xml_node pnode = node.append_child("positionPlayer");
	pnode.append_attribute("x") = app->scene->player->position.x;
	pnode.append_attribute("y") = app->scene->player->position.y;
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