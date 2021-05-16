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
#include "SceneIntro.h"
#include "SceneLose.h"
#include "SceneWin.h"
#include "SceneBattle.h"
#include "SceneDungeon.h"
#include "ModuleParticles.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "DialogSystem.h"
#include "Shop.h"
#include "Defs.h"
#include "Log.h"
#include "HUD.h"
#include "Quests.h"
#include "PlayerStats.h"



Scene::Scene() : Module()
{
	name.Create("scene");	
}

// Destructor
Scene::~Scene(){}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	scale = (int)app->win->GetScale();
	fullscreenRect = new SDL_Rect{ 0, 0, 1280, 720};
	bool ret = true;
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L12b: Create walkability map on map loading
	if (this->active == true)
	{
		// Set Current Scene to TOWN
		currentScene = GameScene::SCENE_TOWN;		

		// EntityManager Start
		app->entityManager->Enable();

		// Dialogue Setup
		app->dialogueSystem->Enable();
		// Setting dialogue to id 0 - Beach Girl
		app->dialogueSystem->id = 0;

		// Loads entities
		npc1 = (NPC1*)app->entityManager->CreateEntity(EntityType::NPC1);		
		npc5 = (NPC5*)app->entityManager->CreateEntity(EntityType::NPC5);
		
		enemy1 = (Enemy1*)app->entityManager->CreateEntity(EntityType::Enemy1);
		enemy2 = (Enemy2*)app->entityManager->CreateEntity(EntityType::Enemy2);
		enemy3 = (Enemy3*)app->entityManager->CreateEntity(EntityType::Enemy3);
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);

		// Starts entities that are on the TOWN
		npc1->Start();
		npc5->Start();
		enemy1->Start();
		enemy2->Start();
		enemy3->Start();
		player->Start();

		// Enables the Map
		app->map->Enable();
		app->hud->Enable();
		app->quests->Enable();
		app->playerStats->Enable();
		player->spiked = false;
		player->onBattle = false;
		app->sceneBattle->battleOn = false;
		app->shop->staticPlayer = false;
		
		// Texture assignations for the scene
		texMenu = app->tex->Load("Assets/Textures/pause_menu.png");
		doorOpenFx = app->audio->LoadFx("Assets/Audio/Fx/door_open.wav");
		doorCloseFx = app->audio->LoadFx("Assets/Audio/Fx/door_close.wav");
		doorKnokFx = app->audio->LoadFx("Assets/Audio/Fx/door_knok.wav");

		guiButtonFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_fx.wav");
		guiButtonMoveFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_move.wav");

		selectorTex = app->tex->Load("Assets/Textures/pointer.png");

		enemyMoving = false;
		stop = false;

		char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
		whiteFont = app->font->Load("Assets/Textures/white_font_mini.png", lookupTable, 1);

		app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
		app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;

		app->sceneBattle->knightDead = false;
		app->sceneBattle->mageDead = false;
		app->sceneBattle->playerDead = false;

		

		if(!app->sceneIntro->posContinue) timer = 0;

		// Loads the map and creates walkability map

		if (app->map->Load("town.tmx") == true)
		{
			int w, h;
			uchar* data = NULL;

			if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

			RELEASE_ARRAY(data);
		}
		// Loads music
		app->audio->PlayMusic("Assets/Audio/Music/main_theme.ogg");

		//Buttons
		btnResume = new GuiButton(1, { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 115, 54, 10 }, "RESUME");
		btnResume->SetObserver(this);

		btnSettings = new GuiButton(2, { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 135, 72, 10 }, "SETTINGS");
		btnSettings->SetObserver(this);

		btnBackIntro = new GuiButton(3, { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 155, 81, 10 }, "BACK MENU");
		btnBackIntro->SetObserver(this);

		btnExit = new GuiButton(4, { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 175, 36, 10 }, "EXIT");
		btnExit->SetObserver(this);

		btnBack = new GuiButton(5, { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 200, 36 ,10 }, "BACK");
		btnBack->SetObserver(this);

		sliderMusicVolume = new GuiSlider(1, { -app->render->camera.x / 3, -app->render->camera.y / 3 , 4, 8 }, "MUSIC VOLUME");
		sliderMusicVolume->SetObserver(this);

		sliderFxVolume = new GuiSlider(2, { -app->render->camera.x / 3, -app->render->camera.y / 3, 4, 8 }, "FX VOLUME");
		sliderFxVolume->SetObserver(this);

		checkBoxFullscreen = new GuiCheckBox(1, { -app->render->camera.x / 3, -app->render->camera.y / 3, 12, 12 }, "FULLSCREEN");
		checkBoxFullscreen->SetObserver(this);

		checkBoxVSync = new GuiCheckBox(2, { -app->render->camera.x / 3, -app->render->camera.y / 3,12,12 }, "VSYNC");
		checkBoxVSync->SetObserver(this);

		pos = 0;
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
	GamePad& pad = app->input->pads[0];	

	if (fromDungeon)
	{
		app->scene->ChangeScene(GameScene::SCENE_BSMITH);
		fromDungeon = false;
	}

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

	//Cap in-game FPS
	//if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) app->capped = !app->capped;
	if (app->sceneBattle->playerRevive)
	{
		app->scene->ChangeScene(GameScene::SCENE_INN);
		app->sceneBattle->playerRevive = false;
	}

	if (player->door == COLLIDER_BLUE)
	{
		app->scene->ChangeScene(GameScene::SCENE_HOUSE1);
		app->audio->PlayFx(doorCloseFx, 0);
		player->door = 0;	
	}	
	else if (player->door == COLLIDER_GREY)
	{
		app->scene->ChangeScene(GameScene::SCENE_BSMITH);
		app->audio->PlayFx(doorCloseFx, 0);
		player->door = 0;
	}
	else if (player->door == COLLIDER_YELLOW)
	{
		app->scene->ChangeScene(GameScene::SCENE_INN);
		app->audio->PlayFx(doorCloseFx, 0);
		player->door = 0;
	}
	if (player->houseDoor == COLLIDER_GREEN || player->houseDoor == COLLIDER_GREEN_HOUSE)
	{
		app->scene->ChangeScene(GameScene::SCENE_TOWN);
		app->audio->PlayFx(doorOpenFx, 0);
		player->houseDoor = 0;
	}
	if (player->door == COLLIDER_PINK)
	{
		app->scene->ChangeScene(GameScene::SCENE_ENTRYDUNGEON);
		app->audio->PlayFx(doorCloseFx, 0);
		player->door = 0;
	}
	if (player->houseDoor == COLLIDER_GREEN_FOREST)
	{
		app->fadeToBlack->FadeToBlk(this, app->sceneDungeon, 30);
		app->fadeToBlack->animId = 1;
	}
	if (player->houseDoor == COLLIDER_BLUE_FOREST)
	{
		app->scene->ChangeScene(GameScene::SCENE_TOWN);
		player->houseDoor = 0;
	}

	if (player->ThereIsHouseClosed() && !knokDone)
	{
		app->audio->PlayFx(doorKnokFx, 0);
		knokDone = true;
		houseClosed = true;
	}

	// Request Load / Save when pressing F6/F5
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && !paused && !pausedSettings) app->LoadGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();

	//SceneWin
	if (player->win)
	{
		app->sceneWin->won = true;
		player->win = true;
		app->fadeToBlack->FadeToBlk(this, app->sceneWin, 30);

		app->render->RestartValues();
	}

	// Pause Menu
	if ((app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.start == true) && !app->scene->player->dialogeOn)
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

	lifesScene = player->lifes;
	sceneCounterKey = player->counterKey;
	sceneCounterCheckpoint = player->counterCheckpoint;
	sceneCounterHeart = player->counterHeart;
	sceneCounterPuzzle = player->counterPuzzle;
	
	// Current Scene Update()
	switch (currentScene)
	{
		case GameScene::SCENE_TOWN:
		{
			// Camera Settings
			if (!paused && !app->hud->bagIsOpen && !app->quests->questsIsOpen && !app->playerStats->statsIsOpen)
			{
				//camera x

				if ((app->render->counter == 0 || player->godModeEnabled) && !player->dialogeOn && !paused)
				{
					if (((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || pad.left) && !mageTxt2 && !knightTxt2 && (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !pad.right) && (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) && !pad.right) && player->position.x > 220 && player->position.x <= 419 && !player->ThereIsLeftWall() && !player->ThereIsNPCLeft() && (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !pad.up) && (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !pad.down)) app->render->camera.x += 3.0f;
					else if (((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) || pad.right) && !mageTxt2 && !knightTxt2 && (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !pad.left) && (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) && !pad.left) && player->position.x > 220 && player->position.x <= 419 && !player->ThereIsRightWall() && !player->ThereIsNPCRight() && (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !pad.up) && (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !pad.down)) app->render->camera.x -= 3.0f;
				}
				//camera y
				if ((app->render->counter == 0 || player->godModeEnabled) && !player->dialogeOn && !paused)
				{

					if (((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) || pad.up) && !mageTxt2 && !knightTxt2 && !app->scene->knightTxt && (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !pad.down) && (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) && !pad.down) && player->position.y > 120 && player->position.y <= 400 && !player->ThereIsTopWall() && !player->ThereIsNPCUp() && (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !pad.left) && (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !pad.right))
					{
						app->render->camera.y += 3.0f;
						app->scene->mageTxt = false;
						app->scene->knightTxt = false;
					}
					else if (((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) || pad.down) && !mageTxt2 && !knightTxt2 && !app->scene->mageTxt && (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !pad.up) && (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) && !pad.up) && player->position.y > 120 && player->position.y <= 400 && !player->ThereIsBottomWall() && !player->ThereIsNPCBelow() && (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !pad.left) && (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !pad.right)) app->render->camera.y -= 3.0f;

				}
			}			

			if (player->position.y <= 120)
			{
				app->render->camera.y = -2;
			}
			if (player->position.y >= 400)
			{
				app->render->camera.y = ((-2 - 409 * 3) + 1280 / 2) - 258;
			}
			if (player->position.x <= 220)
			{
				app->render->camera.x = -35;
			}
			if (player->position.x >= 420)
			{
				app->render->camera.x = -640;
			}
		} break;
		case GameScene::SCENE_ENTRYDUNGEON:
		{
			// Camera Settings
			if (!app->scene->paused && !app->hud->bagIsOpen && !app->quests->questsIsOpen && !app->playerStats->statsIsOpen)
			{
				//camera x

				if ((app->render->counter == 0 || player->godModeEnabled) && !player->dialogeOn && !paused)
				{
					if (((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || pad.left) && (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !pad.right) && (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) && !pad.right) && player->position.x > 220 && player->position.x <= 419 && !player->ThereIsLeftWall() && !player->ThereIsNPCLeft() && (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !pad.up) && (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !pad.down)) app->render->camera.x += 3.0f;
					else if (((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) || pad.right) && (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !pad.left) && (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) && !pad.left) && player->position.x > 220 && player->position.x <= 419 && !player->ThereIsRightWall() && !player->ThereIsNPCRight() && (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !pad.up) && (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !pad.down)) app->render->camera.x -= 3.0f;
				}
				//camera y
				if ((app->render->counter == 0 || player->godModeEnabled) && !player->dialogeOn && !paused)
				{
					if (((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) || pad.up) && (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !pad.down) && (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) && !pad.down) && player->position.y > 120 && player->position.y <= 400 && !player->ThereIsTopWall() && !player->ThereIsNPCUp() && (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !pad.left) && (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !pad.right)) app->render->camera.y += 3.0f;
					else if (((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) || pad.down) && (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !pad.up) && (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) && !pad.up) && player->position.y > 120 && player->position.y <= 400 && !player->ThereIsBottomWall() && !player->ThereIsNPCBelow() && (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !pad.left) && (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !pad.right)) app->render->camera.y -= 3.0f;
				}
			}

			if (player->position.y <= 120)
			{
				app->render->camera.y = -2;
			}
			if (player->position.y >= 400)
			{
				app->render->camera.y = ((-2 - 409 * 3) + 1280 / 2) - 258;
			}
			if (player->position.x <= 220)
			{
				app->render->camera.x = -35;
			}
			if (player->position.x >= 420)
			{
				app->render->camera.x = -640;
			}
		} break;		
	}


	if (paused || pausedSettings)
	{
		if (pos == 0)
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) btnResume->state = GuiControlState::PRESSED;
		}
		if (pos == 1)
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) btnSettings->state = GuiControlState::PRESSED;
		}
		if (pos == 2)
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) btnBackIntro->state = GuiControlState::PRESSED;
		}
		if (pos == 3)
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) btnExit->state = GuiControlState::PRESSED;
		}
		if (pos == 4)
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) btnBack->state = GuiControlState::PRESSED;
		}

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || pad.a) 
		{
			if (pos != 5 && pos != 6) app->audio->PlayFx(guiButtonFx, 0);
			Select(); 
		}
	}
	
	if (pausedSettings)
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || pad.up)
		{
			app->audio->PlayFx(guiButtonMoveFx, 0);
			pos--;
			if (pos < 4) pos = 8;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || pad.down)
		{
			app->audio->PlayFx(guiButtonMoveFx, 0);
			pos++;
			if (pos > 8) pos = 4;
		}
	}
	else if (paused)
	{
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || pad.up)
		{
			app->audio->PlayFx(guiButtonMoveFx, 0);
			pos--;
			if (pos < 0) pos = 3;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || pad.down)
		{
			app->audio->PlayFx(guiButtonMoveFx, 0);
			pos++;
			if (pos > 3) pos = 0;
		}
	}
	if (count <= 301)
	{
		count++;
	}

	//take mage
	if (mageTxt2&&count2 <= 101)
	{
		count2++;
		if (count2 == 101) mageTxt2 = false;
	}

	if (player->position.y > 306&&!mageTkn&&!mageTxt) mageTxt = true;
	if (mageTxt && !mageTkn && (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x))
	{
		mageTxt2 = true;
	}

	//take knight
	if (forest)
	{
		if (knightTxt2 && count3 <= 101)
		{
			count3++;
			if (count3 == 101) knightTxt2 = false;
		}

		if (player->position.y < 103 && player->position.x > 500 && !knightTkn && !knightTxt) knightTxt = true;
		if (knightTxt && !knightTkn && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || pad.x)
		{
			knightTxt2 = true;
		}
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	if (app->sceneIntro->exit == true) return false;

	// Draw map
	app->map->Draw();
	app->map->DrawColliders();
	//app->map->DrawCheckpoint();
	app->map->DrawPuzzle();
	if (!app->map->chestTaken) app->map->DrawChest();
	if (!app->map->swordTaken) app->map->DrawSword();
	if (app->map->chestTaken) app->map->DrawHeart();

	if (mageTxt2)
	{

		if (count2 < 100)
		{
			app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
			app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);

			app->font->DrawText(15, 205, whiteFont, "The mage joined the team!");
		}
		mageTkn = true;
		mageTxt = false;
	}

	if (knightTxt2)
	{

		if (count3 < 100)
		{
			app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
			app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);

			app->font->DrawText(15, 205, whiteFont, "The knight joined the team!");
		}
		knightTkn = true;
		knightTxt = false;
	}

	if (count < 300)
	{
		app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
		app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);

		app->font->DrawText(15, 205, whiteFont, "Press E to talk with people and get quests.");
		app->font->DrawText(15, 220, whiteFont, "Press Q or RB to open quest list.");
	}

	if (quest1message || quest2message || quest3message)
	{
		app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
		app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);

		app->font->DrawText(15, 205, whiteFont, "New quest added to your list!");
	}


	// Draw Road Signal
	if (player->ThereIsLimit())
	{
		app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
		app->render->DrawRectangle({ 10, 590, 1260, 120}, 100, 100, 200, 220, true, false);
		
		app->font->DrawText(90, 205, whiteFont, "This road is too dangerous.");
		app->font->DrawText(90, 220, whiteFont, "  Go back to the village.");
	}

	if (player->ThereIsHouseClosed())
	{
		app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
		app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);
		app->font->DrawText(110, 205, whiteFont, " This house is closed.");
		app->font->DrawText(110, 220, whiteFont, "There is no one inside.");
	}
	else
	{
		knokDone = false;
		houseClosed = false;
	}

	// Pause Menu
	if (pausedSettings)
	{
		app->render->DrawTexture(texMenu, -app->render->camera.x, -app->render->camera.y, fullscreenRect, 3);
		sliderMusicVolume->Draw();
		sliderFxVolume->Draw();
		checkBoxFullscreen->Draw();
		checkBoxVSync->Draw();
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
	if (pos == 0)
	{
		posScaleX = btnResume->bounds.x - 20;
		posScaleY = btnResume->bounds.y - 2;
	}
	if (pos == 1)
	{
		posScaleX = btnSettings->bounds.x - 20;
		posScaleY = btnSettings->bounds.y - 2;
	}
	if (pos == 2)
	{
		posScaleX = btnBackIntro->bounds.x - 20;
		posScaleY = btnBackIntro->bounds.y - 2;
	}
	if (pos == 3)
	{
		posScaleX = btnExit->bounds.x - 20;
		posScaleY = btnExit->bounds.y - 2;
	}
	if (pos == 4)
	{
		posScaleX = btnBack->bounds.x - 20;
		posScaleY = btnBack->bounds.y - 2;
	}
	if (pos == 5)
	{
		posScaleX = sliderMusicVolume->bounds.x - 155;
		posScaleY = sliderMusicVolume->bounds.y + 1;
	}
	if (pos == 6)
	{
		posScaleX = sliderFxVolume->bounds.x - 155;
		posScaleY = sliderFxVolume->bounds.y + 1;
	}
	if (pos == 7)
	{
		posScaleX = checkBoxFullscreen->bounds.x - 140;
		posScaleY = checkBoxFullscreen->bounds.y + 1;
	}
	if (pos == 8)
	{
		posScaleX = checkBoxVSync->bounds.x - 140;
		posScaleY = checkBoxVSync->bounds.y + 1;
	}


	if(paused || pausedSettings) app->render->DrawTexture(selectorTex, posScaleX, posScaleY, NULL);
	return ret;
}

void Scene::Select()
{
	if (pos == 0)
	{
		paused = false;
	}
	else if (pos == 1)
	{
		pausedSettings = true;
		pos = 5;
	}

	else if (pos == 2)
	{
		app->map->CleanUp();
		app->fadeToBlack->FadeToBlk(this, app->sceneIntro, 30);
		app->fadeToBlack->animId = 1;
		paused = false;
		
	}
	else if (pos == 3)
	{		
		app->sceneIntro->exit = true;
	}
	else if (pos == 4)
	{
		pausedSettings = false;
		pos = 0;
	}
	else if (pos == 5)
	{
		//app->audio->ChangeMusicVolume(sliderMusicVolume->ReturnValue());
	}
	else if (pos == 6)
	{
		//app->audio->ChangeFxVolume(sliderFxVolume->ReturnValue());
	}
	else if (pos == 7)
	{
		app->win->fullScreen = !app->win->fullScreen;
		app->win->ChangeScreenSize();
	}
	else if (pos == 8)
	{
		app->vSync = !app->vSync;
	}

}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(pause);
	app->tex->UnLoad(creditText);
	app->tex->UnLoad(selectorTex);
	app->font->UnLoad(whiteFont);
	app->entityManager->DestroyEntity(player);
	app->entityManager->DestroyEntity(enemy);
	app->entityManager->DestroyEntity(flyingEnemy);
	app->entityManager->DestroyEntity(npc1);
	app->entityManager->DestroyEntity(npc2);
	app->entityManager->DestroyEntity(npc3);
	app->entityManager->DestroyEntity(npc4);
	app->entityManager->DestroyEntity(npc5);
	app->entityManager->DestroyEntity(npc7);
	app->entityManager->DestroyEntity(enemy1);
	app->entityManager->DestroyEntity(enemy2);
	app->entityManager->DestroyEntity(enemy3);
	app->entityManager->DestroyEntity(particles);

	app->map->Disable();
	app->entityManager->Disable();
	app->dialogueSystem->Disable();
	app->hud->Disable();
	app->quests->Disable();
	app->playerStats->Disable();

	//RELEASE(btnResume);
	//RELEASE(btnSettings);
	//RELEASE(btnBackIntro);
	//RELEASE(btnExit);
	//RELEASE(btnBack);
	//RELEASE(sliderMusicVolume);
	//RELEASE(sliderFxVolume);
	//RELEASE(checkBoxFullscreen);
	//RELEASE(checkBoxVSync);

	return true;
}

// Change Internal Scene
void Scene::ChangeScene(GameScene nextScene)
{
	LOG("Changing scene");
	
	// Clearing Map
	app->map->CleanUp();

	switch (nextScene)
	{
		case GameScene::SCENE_NONE:
		{
			LOG("ERROR: Scene loaded was none so intro scene loaded instead.");
			ChangeScene(GameScene::SCENE_TOWN);
			break;
		}		
		case GameScene::SCENE_TOWN:
		{
			if (app->map->Load("town.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			forest = false;
			npc1 = (NPC1*)app->entityManager->CreateEntity(EntityType::NPC1);
			npc1->Start();
			//load mage
			if (!mageTkn && !app->sceneBattle->mageRevive)
			{
				npc5 = (NPC5*)app->entityManager->CreateEntity(EntityType::NPC5);
				npc5->Start();
			}

			//npc7 = (NPC7*)app->entityManager->CreateEntity(EntityType::NPC7);
			//npc7->Start();
			//unload knight
			/*if (!knightTkn && !app->sceneBattle->knightRevive)
			{
				npc7->CleanUp();
				app->entityManager->DestroyEntity(npc7);
				npc7 = nullptr;
			}*/

			// Setting dialogue to id 0 Beach Girl and restart dialog system
			app->dialogueSystem->CleanUp();
			app->dialogueSystem->id = 0;
			app->dialogueSystem->Start();
			app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];

			quest1message = false;
			quest2message = false;
			quest3message = false;
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
					player->kDoorTaked = true;


					if (player->doorTaked)
					{
						//player->posMoved = 0;
						player->doorTaked2 = true;
					}
					
					if (player->kDoorTaked)
					{
						//player->posMoved = 0;
						player->kDoorTaked2 = true;
					}
					//player->doorTaked = true;
					//player->doorTakedY = true;
					player->lastPositionX2=player->position.x;
					player->lastPositionY2=player->position.y;

					player->klastPositionX2 = player->position.x;
					player->klastPositionY2 = player->position.y;
					
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
					player->kDoorTaked = true;

					if (player->doorTaked)
					{
						//player->posMoved = 0;
						player->doorTaked2 = true;

					}
					if (player->kDoorTaked)
					{
						//player->posMoved = 0;
						player->kDoorTaked2 = true;
					}
					//player->doorTaked = true;
					//player->doorTakedY = true;
					player->lastPositionX2 = player->position.x;
					player->lastPositionY2 = player->position.y;
					player->klastPositionX2 = player->position.x;
					player->klastPositionY2 = player->position.y;

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
					player->kDoorTaked = true;
					if (player->doorTaked)
					{
						//player->posMoved = 0;
						player->doorTaked2 = true;

					}
					if (player->kDoorTaked)
					{
						//player->posMoved = 0;
						player->kDoorTaked2 = true;
					}
					//player->doorTakedY = true;
					player->lastPositionX2 = player->position.x;
					player->lastPositionY2 = player->position.y;
					player->klastPositionX2 = player->position.x;
					player->klastPositionY2 = player->position.y;
					app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
					app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;

					npc4->CleanUp();
					app->entityManager->DestroyEntity(npc4);
					npc4 = nullptr;

					house = 0;
				} break;
				case 4: 
				{
					app->scene->player->position.x = 588;
					app->scene->player->position.y = 340;
					player->doorTaked = true;
					player->kDoorTaked = true;
					if (player->doorTaked)
					{
						player->doorTaked2 = true;

					}
					if (player->kDoorTaked)
					{
						//player->posMoved = 0;
						player->kDoorTaked2 = true;
					}
					player->lastPositionX2 = player->position.x;
					player->lastPositionY2 = player->position.y;
					player->klastPositionX2 = player->position.x;
					player->klastPositionY2 = player->position.y;
					app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
					app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;										

					house = 0;
				}break;
			}
			currentScene = GameScene::SCENE_TOWN;
		}
		break;
		case GameScene::SCENE_HOUSE1:
		{
			if (app->map->Load("house1.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			forest = false;
			// Unload Beach Girl
			npc1->CleanUp();
			app->entityManager->DestroyEntity(npc1);
			npc1 = nullptr;
			//unload mage
			if (!mageTkn)
			{
				npc5->CleanUp();
				app->entityManager->DestroyEntity(npc5);
				npc5 = nullptr;
			}
			// Unload knight			
			if (!knightTkn && npc7 != nullptr)
			{
				npc7->CleanUp();
				app->entityManager->DestroyEntity(npc7);
				npc7 = nullptr;
			}

			// Creates Fisherman and starts it	
			npc3 = (NPC3*)app->entityManager->CreateEntity(EntityType::NPC3);
			npc3->Start();

			// Setting dialogue to id 2 Fisherman and restart dialog system
			
			app->dialogueSystem->CleanUp();
			app->dialogueSystem->id = 2;
			app->dialogueSystem->Start();
			app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];

			house = 1;
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->scene->player->position.x = 153;
			app->scene->player->position.y = 156;

			player->doorTaked = true;
			player->kDoorTaked = true;
			//player->doorTakedY = true;
			player->lastPositionX2 = player->position.x;
			player->lastPositionY2 = player->position.y;
			player->klastPositionX2 = player->position.x;
			player->klastPositionY2 = player->position.y;

			currentScene = GameScene::SCENE_HOUSE1;
		} break;
		case GameScene::SCENE_BSMITH:
		{
			app->shop->Enable();			

			if (app->map->Load("herreria.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			forest = false;
			// Unload Beach Girl
			npc1->CleanUp();
			app->entityManager->DestroyEntity(npc1);
			npc1 = nullptr;
			// Unload mage
			if (!mageTkn)
			{
				npc5->CleanUp();
				app->entityManager->DestroyEntity(npc5);
				npc5 = nullptr;
			}
			// Unload Kight if not taken
			if (!knightTkn && npc7 != nullptr)
			{
				npc7->CleanUp();
				app->entityManager->DestroyEntity(npc7);
				npc7 = nullptr;
			}
			// Creates Blacksmith and Starts it
			npc2 = (NPC2*)app->entityManager->CreateEntity(EntityType::NPC2);
			npc2->Start();
			
			// Setting dialogue to id 1 Blacksmith and restart dialog system
			app->dialogueSystem->CleanUp();
			app->dialogueSystem->id = 1;
			app->dialogueSystem->Start();
			app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];

			house = 2;
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->scene->player->position.x = 201;
			app->scene->player->position.y = 158;

			player->doorTaked = true;
			player->kDoorTaked = true;
			//player->doorTakedY = true;
			player->lastPositionX2 = player->position.x;
			player->lastPositionY2 = player->position.y;
			player->klastPositionX2 = player->position.x;
			player->klastPositionY2 = player->position.y;

			currentScene = GameScene::SCENE_BSMITH;
		} break;
		case GameScene::SCENE_INN:
		{
			if (app->map->Load("hostal.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			forest = false;
			// Unload Beach Girl
			npc1->CleanUp();
			app->entityManager->DestroyEntity(npc1);
			npc1 = nullptr;

			//unload mage
			if (!mageTkn)
			{
				npc5->CleanUp();
				app->entityManager->DestroyEntity(npc5);
				npc5 = nullptr;
			}
			if (!knightTkn && npc7 != nullptr)
			{
				npc7->CleanUp();
				app->entityManager->DestroyEntity(npc7);
				npc7 = nullptr;
			}
			//load mage
			if (app->sceneBattle->mageRevive)
			{
				npc5 = (NPC5*)app->entityManager->CreateEntity(EntityType::NPC5);
				npc5->Start();
			}
			//load knight
			if (app->sceneBattle->knightRevive)
			{
				npc7 = (NPC7*)app->entityManager->CreateEntity(EntityType::NPC7);
				npc7->Start();
			}

			npc4 = (NPC4*)app->entityManager->CreateEntity(EntityType::NPC4);
			npc4->Start();


			// Setting dialogue to id 3 Posadera and restart dialog system
			app->dialogueSystem->CleanUp();
			app->dialogueSystem->id = 3;
			app->dialogueSystem->Start();
			app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];
			


			house = 3;
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->scene->player->position.x = 217;
			app->scene->player->position.y = 174;

			player->doorTaked = true;
			player->kDoorTaked = true;
			//player->doorTakedY = true;
			player->lastPositionX2 = player->position.x;
			player->lastPositionY2 = player->position.y;
			player->klastPositionX2 = player->position.x;
			player->klastPositionY2 = player->position.y;

			currentScene = GameScene::SCENE_INN;
		} break;
		case GameScene::SCENE_ENTRYDUNGEON:
		{
			if (app->map->Load("dungeonEntry.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}
			forest = true;
			// Unload Beach Girl
			npc1->CleanUp();
			app->entityManager->DestroyEntity(npc1);
			npc1 = nullptr;

			// Unload mage
			if (!mageTkn)
			{
				npc5->CleanUp();
				app->entityManager->DestroyEntity(npc5);
				npc5 = nullptr;
			}

			if (!knightTkn && !app->sceneBattle->knightRevive)
			{
				npc7 = (NPC7*)app->entityManager->CreateEntity(EntityType::NPC7);
				npc7->Start();
			}

			// Setting dialogue to id 0 None
			app->dialogueSystem->CleanUp();
			app->dialogueSystem->id = 0;
			app->dialogueSystem->Start();
			app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];
			house = 4;
			app->render->camera.x = 0;
			app->render->camera.y = 0;

			app->scene->player->position.x = 20;
			app->scene->player->position.y = 53;

			player->lastPositionX2 = player->position.x;
			player->lastPositionY2 = player->position.y;
			player->klastPositionX2 = player->position.x;
			player->klastPositionY2 = player->position.y;

			currentScene = GameScene::SCENE_ENTRYDUNGEON;
		}break;
	}
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
	btnResume->bounds = { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 115, 54, 10 };
	btnSettings->bounds = { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 135, 72, 10 };
	btnBackIntro->bounds = { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 155, 81, 10 };
	btnExit->bounds = { -app->render->camera.x / 3 + 170,  -app->render->camera.y / 3 + 175, 36, 10 };
	btnBack->bounds = { -app->render->camera.x / 3 + 190,  -app->render->camera.y / 3 + 205, 36, 10 };

	sliderMusicVolume->bounds = { -app->render->camera.x / 3 + 250,  -app->render->camera.y / 3 + 115, 4,8 };
	sliderMusicVolume->boundsSlider = { -app->render->camera.x / 3 + 250,  -app->render->camera.y / 3 + 115, 4,8 };

	sliderFxVolume->bounds = { -app->render->camera.x / 3 + 250,  -app->render->camera.y / 3 + 135, 4,8 };
	sliderFxVolume->boundsSlider = { -app->render->camera.x / 3 + 250,  -app->render->camera.y / 3 + 135, 4,8 };

	checkBoxFullscreen->bounds = { -app->render->camera.x / 3 + 234,  -app->render->camera.y / 3 + 155, 12,12 };
	checkBoxVSync->bounds = { -app->render->camera.x / 3 + 234,  -app->render->camera.y / 3 + 175, 12,12 };
}

//bool Scene::OnGuiMouseClickEvent(GuiControl* control)
//{
//	switch (control->type)
//	{
//	case GuiControlType::BUTTON:
//	{
//		if (control->id == 1)
//		{
//			paused = false;
//			
//		}
//		else if (control->id == 2) pausedSettings = true;
//		else if (control->id == 3)
//		{
//			app->map->CleanUp();
//			//if (currentScene != GameScene::SCENE_TOWN)
//			//{
//			//	currentScene = GameScene::SCENE_TOWN;
//			//}
//			app->fadeToBlack->FadeToBlk(this, app->sceneIntro, 30);
//			app->fadeToBlack->animId = 1;
//			paused = false;
//		}
//		else if (control->id == 4)
//		{
//			if (app->scene->player != nullptr)
//			{
//				app->scene->player->position.x = 350;
//				app->scene->player->position.y = 875;
//				app->SaveGameRequest();
//			}
//			app->sceneIntro->exit = true;
//		}
//		else if (control->id == 5) pausedSettings = false;
//		break;
//	}
//	case GuiControlType::SLIDER:
//	{
//		if (control->id == 1) app->audio->ChangeMusicVolume(sliderMusicVolume->ReturnValue());
//		else if (control->id == 2) app->audio->ChangeFxVolume(sliderFxVolume->ReturnValue());
//		break;
//	}
//	case GuiControlType::CHECKBOX:
//	{
//		if (control->id == 1)
//		{
//			app->win->fullScreen = !app->win->fullScreen;
//			app->win->ChangeScreenSize();
//		}
//		else if (control->id == 2) app->vSync = !app->vSync;
//		break;
//	}
//	default: break;
//	}
//
//	return true;
//}