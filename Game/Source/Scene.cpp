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
#include "ModuleParticles.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "DialogSystem.h"
#include "Defs.h"
#include "Log.h"



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

		// Setting dialogue to id 0 - Beach Girl
		app->dialogueSystem->id = 0;

		// Loads entities
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		npc1 = (NPC1*)app->entityManager->CreateEntity(EntityType::NPC1);		
		npc4 = (NPC4*)app->entityManager->CreateEntity(EntityType::NPC4);
		enemy1 = (Enemy1*)app->entityManager->CreateEntity(EntityType::Enemy1);
		enemy2 = (Enemy2*)app->entityManager->CreateEntity(EntityType::Enemy2);
		enemy3 = (Enemy3*)app->entityManager->CreateEntity(EntityType::Enemy3);

		// Starts entities that are on the TOWN
		player->Start();
		npc1->Start();
		npc4->Start();
		enemy1->Start();
		enemy2->Start();
		enemy3->Start();

		// Enables the Map
		app->map->Enable();
		player->spiked = false;
		player->onBattle = false;
		// Texture assignations for the scene
		texMenu = app->tex->Load("Assets/Textures/pause_menu.png");

		char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
		whiteFont = app->font->Load("Assets/Textures/white_font_mini.png", lookupTable, 1);

		app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
		app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;


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
		app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

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

		sliderMusicVolume = new GuiSlider(1, { -app->render->camera.x / 3, -app->render->camera.y / 3 , 10, 28 }, "MUSIC VOLUME");
		sliderMusicVolume->SetObserver(this);

		sliderFxVolume = new GuiSlider(2, { -app->render->camera.x / 3, -app->render->camera.y / 3, 10, 28 }, " FX VOLUME");
		sliderFxVolume->SetObserver(this);

		checkBoxFullscreen = new GuiCheckBox(1, { -app->render->camera.x / 3, -app->render->camera.y / 3, 40, 40 }, "FULLSCREEN");
		checkBoxFullscreen->SetObserver(this);

		checkBoxVSync = new GuiCheckBox(2, { -app->render->camera.x / 3, -app->render->camera.y / 3,40,40 }, "   VSYNC");
		checkBoxVSync->SetObserver(this);
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
	//View Colliders
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) guiColliders = !guiColliders;
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		// Print colliders manually implemented
		app->map->colliders = !app->map->colliders;

		// Print colliders from Collision Modulet
		app->debug = !app->debug;
	}
	//God Mode
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) player->godModeEnabled = !player->godModeEnabled;

	//Cap in-game FPS
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) app->capped = !app->capped;
	if (player->door == COLLIDER_BLUE)
	{
		app->scene->ChangeScene(GameScene::SCENE_HOUSE1);
		player->door = 0;
	}
	else if (player->door == COLLIDER_GREY)
	{
		app->scene->ChangeScene(GameScene::SCENE_BSMITH);
		player->door = 0;
	}
	else if (player->door == COLLIDER_YELLOW)
	{
		app->scene->ChangeScene(GameScene::SCENE_INN);
		player->door = 0;
	}
	if (player->houseDoor == COLLIDER_GREEN || player->houseDoor == COLLIDER_GREEN_HOUSE)
	{
		app->scene->ChangeScene(GameScene::SCENE_TOWN);
		player->houseDoor = 0;
	}

	// Current Scene Update()
	switch (currentScene)
	{
		case GameScene::SCENE_TOWN:
		{
			// Camera Settings
			if (!app->scene->paused)
			{
				//camera x

				if ((app->render->counter == 0 || player->godModeEnabled) && !player->dialogeOn && !paused)
				{
					if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) && player->position.x > 220 && player->position.x <= 419 && !player->ThereIsLeftWall() && !player->ThereIsNPCLeft()) app->render->camera.x += 3.0f;
					else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) && player->position.x > 220 && player->position.x <= 419 && !player->ThereIsRightWall() && !player->ThereIsNPCRight()) app->render->camera.x -= 3.0f;
				}
				//camera y
				if ((app->render->counter == 0 || player->godModeEnabled) && !player->dialogeOn && !paused)
				{
					if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) && player->position.y > 120 && player->position.y <= 400 && !player->ThereIsTopWall() && !player->ThereIsNPCUp()) app->render->camera.y += 3.0f;
					else if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) && player->position.y > 120 && player->position.y <= 400 && !player->ThereIsBottomWall() && !player->ThereIsNPCBelow()) app->render->camera.y -= 3.0f;
				}
			}			

			if (player->position.y <= 120)
			{
				app->render->camera.y = -2;
			}
			if (player->position.y >= 409)
			{
				app->render->camera.y = ((-2 - 409 * 3) + 1280 / 2) - 258;
			}
			if (player->position.x <= 220)
			{
				app->render->camera.x = -35;
			}
			if (player->position.x >= 426)
			{
				app->render->camera.x = -640;
			}

			// Request Load / Save when pressing F6/F5
			if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && !paused && !pausedSettings) app->LoadGameRequest();

			if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();			



			if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
			{
				app->sceneBattle->battleOn = true;
				app->fadeToBlack->FadeToBlk(this, app->sceneBattle, 30);
			}


			//SceneWin
			if (player->win)
			{
				app->sceneWin->won = true;
				player->win = true;
				app->fadeToBlack->FadeToBlk(this, app->sceneWin, 30);

				app->render->RestartValues();
			}

			// Pause Menu
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

			lifesScene = player->lifes;
			sceneCounterKey = player->counterKey;
			sceneCounterCheckpoint = player->counterCheckpoint;
			sceneCounterHeart = player->counterHeart;
			sceneCounterPuzzle = player->counterPuzzle;

			if (app->sceneIntro->exit == true) return false;		

		} break;
		case GameScene::SCENE_HOUSE1:
		{
			
		} break;
		case GameScene::SCENE_BSMITH:
		{
			
		} break;
		case GameScene::SCENE_INN:
		{
			
		} break;
	}
	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	// Draw map
	app->map->Draw();
	app->map->DrawColliders();
	app->map->DrawCheckpoint();
	app->map->DrawPuzzle();
	if (!app->map->chestTaken) app->map->DrawChest();
	if (app->map->chestTaken) app->map->DrawHeart();

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

	// Pause Menu
	if (pausedSettings)
	{
		app->render->DrawRectangle({ -app->render->camera.x / 3, -app->render->camera.y / 3  ,500,500 }, 0, 0, 0, 120);
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

	// GUI Colliders
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
	app->tex->UnLoad(pause);
	app->tex->UnLoad(creditText);
	app->font->UnLoad(whiteFont);
	app->entityManager->DestroyEntity(player);
	app->entityManager->DestroyEntity(enemy);
	app->entityManager->DestroyEntity(flyingEnemy);
	app->entityManager->DestroyEntity(npc1);
	app->entityManager->DestroyEntity(npc2);
	app->entityManager->DestroyEntity(npc3);
	app->entityManager->DestroyEntity(npc4);
	app->entityManager->DestroyEntity(enemy1);
	app->entityManager->DestroyEntity(enemy2);
	app->entityManager->DestroyEntity(enemy3);
	app->entityManager->DestroyEntity(particles);

	delete btnResume;
	delete btnSettings;
	delete btnBackIntro;
	delete btnExit;
	delete btnBackSettings;
	delete btnBack;
	delete sliderMusicVolume;
	delete sliderFxVolume;
	delete checkBoxFullscreen;
	delete checkBoxVSync;


	return true;
}

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
			
			// Setting dialogue to id 0 Beach Girl and restart dialog system
			app->dialogueSystem->Disable();
			app->dialogueSystem->id = 0;
			app->dialogueSystem->Enable();
			app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];

			// Deteting in which house the player was
			// Reposition the player
			switch (house)
			{
				// 0 - None
				// 1 - House1
				// 2 - Blacksmith
				// 3 - Inn
				case 1:
				{
					app->scene->player->position.x = 144;
					app->scene->player->position.y = 99;
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
					app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
					app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;

					
					app->entityManager->DestroyEntity(npc2);

					house = 0;
				} break;
				case 3:
				{
					app->scene->player->position.x = 401;
					app->scene->player->position.y = 320;
					app->render->camera.x = (-20 - player->position.x * 3) + 1280 / 2;
					app->render->camera.y = (-2 - player->position.y * 3) + 720 / 2;
					house = 0;
				} break;			
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

			// Creates Fisherman and starts it	
			npc3 = (NPC3*)app->entityManager->CreateEntity(EntityType::NPC3);
			npc3->Start();

			// Setting dialogue to id 2 Fisherman and restart dialog system
			app->dialogueSystem->Disable();
			app->dialogueSystem->id = 2;
			app->dialogueSystem->Enable();
			app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];

			house = 1;
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->scene->player->position.x = 153;
			app->scene->player->position.y = 156;

			currentScene = GameScene::SCENE_HOUSE1;
		} break;
		case GameScene::SCENE_BSMITH:
		{
			if (app->map->Load("herreria.tmx") == true)
			{
				int w, h;
				uchar* data = NULL;

				if (app->map->CreateWalkabilityMap(w, h, &data)) app->pathfinding->SetMap(w, h, data);

				RELEASE_ARRAY(data);
			}

			// Creates Blacksmith and Starts it
			npc2 = (NPC2*)app->entityManager->CreateEntity(EntityType::NPC2);
			npc2->Start();

			// Setting dialogue to id 1 Blacksmith and restart dialog system
			app->dialogueSystem->Disable();
			app->dialogueSystem->id = 1;
			app->dialogueSystem->Enable();
			app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];

			house = 2;
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->scene->player->position.x = 201;
			app->scene->player->position.y = 158;

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

			// Setting dialogue to id 3 Posadera and restart dialog system
			app->dialogueSystem->Disable();
			app->dialogueSystem->id = 3;
			app->dialogueSystem->Enable();
			app->dialogueSystem->currentNode = app->dialogueSystem->dialogueTrees[app->dialogueSystem->id]->dialogueNodes[0];

			house = 3;
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			app->scene->player->position.x = 217;
			app->scene->player->position.y = 174;
			currentScene = GameScene::SCENE_INN;
		} break;
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
	app->tex->UnLoad(app->scene->player->texPlayer);
	btnResume->bounds = { -app->render->camera.x / 3 + 170,-app->render->camera.y / 3 + 115, 70, 12 };
	btnSettings->bounds = { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 135, 70, 12 };
	btnBackIntro->bounds = { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 155, 70, 12 };
	btnExit->bounds = { -app->render->camera.x / 3 + 170,  -app->render->camera.y / 3 + 175, 70, 12 };
	btnBack->bounds = { -app->render->camera.x / 3 + 170, -app->render->camera.y / 3 + 200,70 ,12 };
	sliderMusicVolume->bounds = { -app->render->camera.x / 3, -app->render->camera.y / 3 , 10, 28 };
	sliderFxVolume->bounds = { -app->render->camera.x / 3, -app->render->camera.y / 3 , 10, 28 };
	checkBoxFullscreen->bounds = { -app->render->camera.x / 3, -app->render->camera.y / 3 , 40, 40 };
	checkBoxVSync->bounds = { -app->render->camera.x / 3, -app->render->camera.y / 3 , 40, 40 };
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
			app->scene->player->texPlayer= app->tex->Load("Assets/Textures/main_character.png");
			
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