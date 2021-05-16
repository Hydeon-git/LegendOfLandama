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
#include "HUD.h"
#include "Quests.h"
#include "PlayerStats.h"

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
		app->entityManager->Enable();
		app->hud->Enable();
		app->quests->Enable();
		app->playerStats->Enable();

		// Music
		app->audio->PlayMusic("Assets/Audio/Music/dungeon_music.ogg");

		guiButtonFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_fx.wav");
		guiButtonMoveFx = app->audio->LoadFx("Assets/Audio/Fx/gui_button_move.wav");

		// Entities
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);

		npc5 = (NPC5*)app->entityManager->CreateEntity(EntityType::NPC5);
		npc7 = (NPC7*)app->entityManager->CreateEntity(EntityType::NPC7);

		app->scene->player->dead = false;
		player->Start();

		npc5->Start();
		npc7->Start();			
		
		app->scene->mageTkn=true;
		app->scene->knightTkn=true;

		app->scene->player->kDoorTaked = false; 
		app->scene->player->doorTaked = false; 

		app->scene->player->kDoorTaked = false;
		app->scene->player->doorTaked = false;


		// Texture assignations for the scene dungeon
		texMenu = app->tex->Load("Assets/Textures/pause_menu.png");

		selectorTex = app->tex->Load("Assets/Textures/pointer.png");

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
	}
	return ret;
}

bool SceneDungeon::Update(float dt)
{
	GamePad& pad = app->input->pads[0];

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

	if (!paused && !app->hud->bagIsOpen && !app->quests->questsIsOpen)
	{
		// Camera x
		if ((app->render->counter == 0 || player->godModeEnabled))
		{
			if (((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) || pad.left)
				&& (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !pad.right)
				&& (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) && !pad.right)
				&& (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !pad.up)
				&& (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !pad.down)
				&& !player->ThereIsLeftWall() 
				&& !player->ThereIsNPCLeft())
			{
				app->render->camera.x += 3.0f;
			}
				

			else if (((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) || pad.right)
				&& (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !pad.left)
				&& (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) && !pad.left)
				&& (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !pad.up)
				&& (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !pad.down)
				&& !player->ThereIsRightWall()
				&& !player->ThereIsNPCRight())
			{
				app->render->camera.x -= 3.0f;
			}
				
		}
		// Camera y
		if ((app->render->counter == 0 || player->godModeEnabled))
		{

			if (((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) || pad.up)
				&& (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !pad.down)
				&& (!(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) && !pad.down)
				&& !player->ThereIsTopWall()
				&& !player->ThereIsNPCUp()
				&& (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !pad.left)
				&& (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !pad.right))
			{
				app->render->camera.y += 3.0f;
			}
			else if (((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) || pad.down)
				&& (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !pad.up)
				&& (!(app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) && !pad.up)
				&& !player->ThereIsBottomWall()
				&& !player->ThereIsNPCBelow()
				&& (!(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !pad.left)
				&& (!(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !pad.right))
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
	if ((app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.start) && !app->scene->player->dialogeOn)
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

	if (app->map->chestOpened)
	{
		if (countChest <= 201)
		{
			countChest++;
		}
	}

	if (app->map->swordOpened)
	{
		if (countSword <= 401)
		{
			countSword++;
		}
	}
	return true;
}

// Update: draw background
bool SceneDungeon::PostUpdate()
{
	bool ret = true;
	if (app->sceneIntro->exit == true) ret = false;

	app->map->Draw();

	app->map->DrawDoor();
	app->map->DrawDoorMid();
	app->map->DrawChest();
	app->map->DrawSword();
	app->map->DrawLevers();
	if (!app->map->puzzle1DungeonDone)
	{
		app->map->DrawWalls2Dungeon();
	}
	if (app->map->buttonFloorPressed)
	{
		app->map->DrawFloor2Dungeon();
	}
	

	if (app->map->chestOpened)
	{
		if (countChest < 200)
		{
			app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
			app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);

			app->font->DrawText(15, 205, whiteFont, "You found 1 Potion");
			app->font->DrawText(15, 220, whiteFont, "20 coins added to your inventary");

		}
	}


	if (app->map->swordOpened)
	{
		if (countSword < 200)
		{
			app->render->DrawRectangle({ 0, 30, 1280, 140 }, 0, 0, 0, 220, true, false);
			app->render->DrawRectangle({ 10, 40, 1260, 120 }, 100, 100, 200, 220, true, false);


			app->font->DrawText(160, 20, whiteFont, "CONTRATULATIONS!");
			app->font->DrawText(150, 35, whiteFont, "YOU FOUND THE SWORD");

		}
		else if (countSword < 400)
		{
			app->render->DrawRectangle({ 0, 30, 1280, 140 }, 0, 0, 0, 220, true, false);
			app->render->DrawRectangle({ 10, 40, 1260, 120 }, 100, 100, 200, 220, true, false);
			app->font->DrawText(30, 20, whiteFont, "200 coins added to your inventary");
			app->font->DrawText(30, 35, whiteFont, "going to deliver the sword...");
		}
		else
		{
			countSword = 500;
			app->fadeToBlack->FadeToBlk(this, app->scene, 30);
			app->fadeToBlack->animId = 1;
			app->scene->fromDungeon = true;
		}

	}

	//spikes
	if (!app->map->lever1Pressed && !app->map->lever2Pressed && !app->map->lever3Pressed && !app->map->leverFinalPressed)
	{
		app->map->DrawSpikes1();
		app->map->DrawSpikes2();
		app->map->DrawSpikes3();
	}
	else if (app->map->lever1Pressed && !app->map->lever2Pressed && !app->map->lever3Pressed && !app->map->leverFinalPressed)
	{
		//app->map->DrawSpikes1();
		app->map->DrawSpikes2();
		app->map->DrawSpikes3();
	}
	else if (app->map->lever1Pressed && app->map->lever2Pressed && !app->map->lever3Pressed && !app->map->leverFinalPressed)
	{
		app->map->DrawSpikes1();
		//app->map->DrawSpikes2();
		app->map->DrawSpikes3();
	}
	else if (app->map->lever1Pressed && app->map->lever2Pressed && app->map->lever3Pressed && !app->map->leverFinalPressed)
	{
		//app->map->DrawSpikes1();
		//app->map->DrawSpikes2();
		app->map->DrawSpikes3();
	}
	//else if ((!app->map->lever1Pressed && !app->map->lever2Pressed && !app->map->lever3Pressed && app->map->leverFinalPressed) || (app->map->lever1Pressed && !app->map->lever2Pressed && !app->map->lever3Pressed && app->map->leverFinalPressed))
	//{
	//	app->map->DrawSpikes1();
	//	app->map->DrawSpikes2();
	//	app->map->DrawSpikes3();
	//}

	if (!app->map->blueKeyTaken)
	{
		app->map->DrawKeyBlue();
	}
	if (!app->map->redKeyTaken)
	{
		app->map->DrawKeyRed();
	}
	if (!app->map->greenKeyTaken)
	{
		app->map->DrawKeyGreen();
	}
	if (!app->map->yellowKeyTaken)
	{
		app->map->DrawKeyYellow();
	}

	app->map->DrawDoorRed();
	app->map->DrawDoorBlue1();
	app->map->DrawDoorBlue2();
	app->map->DrawDoorGreen();
	app->map->DrawDoorYellow();


	app->map->DrawColliders();

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


	if (paused || pausedSettings) app->render->DrawTexture(selectorTex, posScaleX, posScaleY, NULL);
	return ret;
}

bool SceneDungeon::CleanUp()
{
	app->font->UnLoad(whiteFont);
	app->tex->UnLoad(winText);	
	app->font->UnLoad(whiteFont);
	app->tex->UnLoad(selectorTex);

	app->entityManager->DestroyEntity(player);	
	app->entityManager->DestroyEntity(npc5);
	app->entityManager->DestroyEntity(npc7);

	app->map->Disable();
	app->entityManager->Disable();
	app->hud->Disable();
	app->quests->Disable();
	app->playerStats->Disable();
	return true;
}

void SceneDungeon::Pause()
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

//bool SceneDungeon::OnGuiMouseClickEvent(GuiControl* control)
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
//			app->fadeToBlack->FadeToBlk(this, app->sceneIntro, 30);
//			app->fadeToBlack->animId = 1;
//			paused = false;
//		}
//		else if (control->id == 4)
//		{			
//			app->sceneIntro->exit = true;
//		}
//		else if (control->id == 5) pausedSettings = false;
//		break;
//	}
//	/*case GuiControlType::SLIDER:
//	{
//		if (control->id == 1) app->audio->ChangeMusicVolume(sliderMusicVolume->ReturnValue());
//		else if (control->id == 2) app->audio->ChangeFxVolume(sliderFxVolume->ReturnValue());
//		break;
//	}*/
//	case GuiControlType::CHECKBOX:
//	{
//		if (control->id == 1)
//		{
//			app->win->fullScreen = !app->win->fullScreen;
//			app->win->ChangeScreenSize();
//		}
//		//else if (control->id == 2) app->vSync = !app->vSync;
//		break;
//	}
//	default: break;
//	}
//
//	return true;
//}

void SceneDungeon::Select()
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
