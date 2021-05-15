#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Shop.h"
#include "Scene.h"
#include "SceneDungeon.h"
#include "SceneLose.h"
#include "SceneBattle.h"
#include "Enemy.h"
#include "Enemy1.h"
#include "NPC1.h"
#include "NPC4.h"
#include "NPC5.h"
#include "NPC7.h"
#include "ModuleParticles.h"
#include "FlyingEnemy.h"
#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"
#include "SceneIntro.h"
#include "SceneWin.h"
#include "DialogSystem.h"
#include "HUD.h"
#include "Quests.h"


#define INVENTORY_SIZE 13

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");
	
	if (app->sceneIntro->startClicked)
	{
		position.x = 200;
		position.y = 100;
		//position.x = 548;
		//position.y = 129;

		lastPositionX2 = 200;
		lastPositionY2 = 100;

		klastPositionX2 = 200;
		klastPositionY2 = 100;

		app->sceneIntro->startClicked = false;
		app->sceneIntro->posContinue = false;
		app->sceneWin->won = false;
		app->sceneLose->lost = false;
	}
	else if (app->sceneBattle->battleOn)
	{
		position.x = 140;
		position.y = 115;

	}
	else if (app->sceneBattle->playerRevive)
	{
		position.x = 228;
		position.y = 139;
	}
	else if (app->sceneBattle->battleEnd&& !app->sceneBattle->playerRevive)
	{
		position.x = 484;
		position.y = 344;

		lastPositionX2 = 484;
		lastPositionY2 = 344;

		klastPositionX2 = 484;
		klastPositionY2 = 344;
		app->sceneBattle->battleEnd = false;
	}
	else if (app->sceneDungeon->currentScene == DungeonScene::SCENE_HALL)
	{
		position.x = 232;
		position.y = 381;
	}
	else
	{
		app->LoadGameRequest();
		if (app->scene->player->position.x == 938 && app->scene->player->position.y == 171)
		{
			loaded = true;
		}
	}

	//idlanim
	idlAnim.PushBack({ 0, 0, 15, 16 });
	idlAnim.PushBack({ 16, 0, 16, 16 });
	idlAnim.speed = 0.02f;

	//move right
	rightAnim.PushBack({ 0, 0, 16, 16 });
	rightAnim.PushBack({ 16, 0, 16, 16 });
	rightAnim.PushBack({ 32, 0, 16, 16 });
	rightAnim.PushBack({ 48, 0, 16, 16 });
	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 64, 16, 16, 16 });
	leftAnim.PushBack({ 48, 16, 16, 16 });
	leftAnim.PushBack({ 32, 16, 16, 16 });
	leftAnim.PushBack({ 16, 16, 16, 16 });
	leftAnim.speed = 0.1f;
	
	//move up
	upAnim.PushBack({ 64, 32, 16, 16 });
	upAnim.PushBack({ 48, 32, 16, 16 });
	upAnim.PushBack({ 32, 32, 16, 16 });
	upAnim.PushBack({ 16, 32, 16, 16 });
	upAnim.speed = 0.1f;

	//deathAnim
	deathAnim.PushBack({ 0, 595, 64, 85 });
	deathAnim.PushBack({ 0, 1100, 64, 85 });
	deathAnim.speed = 0.02f;
	deathAnim.loop = false;

	for (int i = 0; i < 15; ++i)
	{
		inventory[i] = nullptr;

	}
}

// Destructor
Player::~Player()
{}

bool Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

bool Player::Start()
{
	if (this->active == true)
	{
		coins = 20;
		dead = false;
		spiked = false;
		win = false;

		if (!app->sceneIntro->posContinue)
		{
			app->map->keyTaken = false;
			app->map->chestTaken = false;
			app->map->heartTaken = false;
			app->map->puzzleTaken = false;
			app->map->puzzle1DungeonDone = false;
			app->map->buttonFloorPressed = false;
			app->map->chestOpened = false;
			lifes = 3;
			counterKey = 0;
			counterCheckpoint = 0;
			counterHeart = 0;
			counterPuzzle = 0;
		}
		else
		{
			counterKey = app->scene->sceneCounterKey;
			counterCheckpoint = app->scene->sceneCounterCheckpoint;
			counterHeart = app->scene->sceneCounterHeart;
			counterPuzzle = app->scene->sceneCounterPuzzle;
			lifes = app->scene->lifesScene;
		}

		// Texture & Animations Load
		texPlayer = app->tex->Load("Assets/Textures/main_character.png");

		playerDeathFx = app->audio->LoadFx("Assets/Audio/Fx/death_sound.wav");
		itemTakenFx = app->audio->LoadFx("Assets/Audio/Fx/item.wav");
		keyTakenFx = app->audio->LoadFx("Assets/Audio/Fx/key.wav");
		heartFx = app->audio->LoadFx("Assets/Audio/Fx/heart.wav");
		fireFx = app->audio->LoadFx("Assets/Audio/Fx/fire.wav");
		talkFx = app->audio->LoadFx("Assets/Audio/Fx/huh.wav");

		chestFx = app->audio->LoadFx("Assets/Audio/Fx/chest_sound.ogg");
		leverFx = app->audio->LoadFx("Assets/Audio/Fx/lever_open.wav");
		leverErrorFx = app->audio->LoadFx("Assets/Audio/Fx/lever_error.wav");

		buttonFx = app->audio->LoadFx("Assets/Audio/Fx/button_press.wav");
		puzzleSolvedFx = app->audio->LoadFx("Assets/Audio/Fx/puzzle_solved.wav");
		currentAnimation = &idlAnim;

		lastPositionX = position.x;
		lastPositionY = position.y;

		klastPositionX = position.x;
		klastPositionY = position.y;

		doorTaked = true;
		kDoorTaked = true;
		//doorTakedY = true;
	}

	return true;
}

bool Player::Update(float dt)
{
	if (loaded)
	{
		app->render->camera.x = -588;
		app->render->camera.y = -99;
		loaded = false;
	}
	if (!spiked && !dead)
	{
		//currentAnimation = &idlAnim;

		if (ThereIsDoor() && app->map->keyTaken) win = true;
		else
		{
			if (!app->scene->paused && !app->sceneDungeon->paused && !dialogeOn && !onBattle && !app->shop->staticPlayer && !app->hud->bagIsOpen && !app->quests->questsIsOpen) 
			{
				if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
				{
					if (!ThereIsTopWall() && !ThereIsNPCUp())
					{
						position.y -= speed;
						currentAnimation = &upAnim;
						if (doorTaked) posMoved++;
						if (kDoorTaked) kposMoved++;
						
						for (int i = 0; i < 25; i++)
						{
							if (i == 24) {
								lastPosY[0] = position.y;
							}
							else
							{
								lastPosY[24 - i] = lastPosY[23 - i];
							}
						}
						if(lastPosY!=0) lastPositionY = lastPosY[24];

						for (int i = 0; i < 25; i++)
						{
							if (i == 24) {
								lastPosX[0] = position.x;
							}
							else
							{
								lastPosX[24 - i] = lastPosX[23 - i];
							}
						}
						if (lastPosY != 0) lastPositionX = lastPosX[24];


						for (int i = 0; i < 50; i++)
						{
							if (i == 49) {
								klastPosY[0] = position.y;
							}
							else
							{
								klastPosY[49 - i] = klastPosY[48 - i];
							}
						}
						if (klastPosY != 0) klastPositionY = klastPosY[49];

						for (int i = 0; i < 50; i++)
						{
							if (i == 49) {
								klastPosX[0] = position.x;
							}
							else
							{
								klastPosX[49 - i] = klastPosX[48 - i];
							}
						}
						if (klastPosX != 0) klastPositionX = klastPosX[49];


					}
				}
				if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
				{
					if (!ThereIsBottomWall() && !ThereIsNPCBelow())
					{
						position.y += speed;
						currentAnimation = &leftAnim;
						if (doorTaked) posMoved++;
						if (kDoorTaked) kposMoved++;
						for (int i = 0; i < 25; i++)
						{
							if (i == 24) {
								lastPosY[0] = position.y;
							}
							else
							{
								lastPosY[24 - i] = lastPosY[23 - i];
							}
						}
						if (lastPosY != 0) lastPositionY = lastPosY[24];

						for (int i = 0; i < 25; i++)
						{
							if (i == 24) {
								lastPosX[0] = position.x;
							}
							else
							{
								lastPosX[24 - i] = lastPosX[23 - i];
							}
						}
						
						if (lastPosX != 0) lastPositionX = lastPosX[24];


						for (int i = 0; i < 50; i++)
						{
							if (i == 49) {
								klastPosY[0] = position.y;
							}
							else
							{
								klastPosY[49 - i] = klastPosY[48 - i];
							}
						}
						if (klastPosY != 0) klastPositionY = klastPosY[49];

						for (int i = 0; i < 50; i++)
						{
							if (i == 49) {
								klastPosX[0] = position.x;
							}
							else
							{
								klastPosX[49 - i] = klastPosX[48 - i];
							}
						}
						if (klastPosX != 0) klastPositionX = klastPosX[49];


					}
				}

				if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT))
				{
					if (!ThereIsLeftWall() && !ThereIsNPCLeft())
					{
						position.x -= speed;
						currentAnimation = &leftAnim;
						if (doorTaked) posMoved++;
						if (kDoorTaked) kposMoved++;
						for (int i = 0; i < 25; i++)
						{
							if (i == 24) {
								lastPosY[0] = position.y;
							}
							else
							{
								lastPosY[24 - i] = lastPosY[23 - i];
							}
						}
						if (lastPosY != 0) lastPositionY = lastPosY[24];

						for (int i = 0; i < 25; i++)
						{
							if (i == 24) {
								lastPosX[0] = position.x;
							}
							else
							{
								lastPosX[24 - i] = lastPosX[23 - i];
							}
						}
						if (lastPosX != 0) lastPositionX = lastPosX[24];

						for (int i = 0; i < 50; i++)
						{
							if (i == 49) {
								klastPosY[0] = position.y;
							}
							else
							{
								klastPosY[49 - i] = klastPosY[48 - i];
							}
						}
						if (klastPosY != 0) klastPositionY = klastPosY[49];

						for (int i = 0; i < 50; i++)
						{
							if (i == 49) {
								klastPosX[0] = position.x;
							}
							else
							{
								klastPosX[49 - i] = klastPosX[48 - i];
							}
						}
						if (klastPosX != 0) klastPositionX = klastPosX[49];



					}
				}
				else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT))
				{
					if (!ThereIsRightWall() && !ThereIsNPCRight())
					{
						position.x += speed;
						currentAnimation = &rightAnim;
						if (doorTaked) posMoved++;
						if (kDoorTaked) kposMoved++;
						for (int i = 0; i < 25; i++)
						{
							if (i == 24) {
								lastPosY[0] = position.y;
							}
							else
							{
								lastPosY[24 - i] = lastPosY[23 - i];
							}
						}
						if (lastPosY != 0) lastPositionY = lastPosY[24];

						for (int i = 0; i < 25; i++)
						{
							if (i == 24) {
								lastPosX[0] = position.x;
							}
							else
							{
								lastPosX[24 - i] = lastPosX[23 - i];
							}
						}
						if (lastPosX != 0) lastPositionX = lastPosX[24];


						for (int i = 0; i < 50; i++)
						{
							if (i == 49) {
								klastPosY[0] = position.y;
							}
							else
							{
								klastPosY[49 - i] = klastPosY[48 - i];
							}
						}
						if (klastPosY != 0) klastPositionY = klastPosY[49];

						for (int i = 0; i < 50; i++)
						{
							if (i == 49) {
								klastPosX[0] = position.x;
							}
							else
							{
								klastPosX[49 - i] = klastPosX[48 - i];
							}
						}
						if (klastPosX != 0) klastPositionX = klastPosX[49];


					}
				}
			}

			if (ThereIsNPC()==1 || ThereIsNPC() == 4 || ThereIsNPCUp() ||ThereIsNPCBelow() || ThereIsNPCLeft() || ThereIsNPCRight())
			{
				if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
				{
					dialogeOn = true;
					app->audio->PlayFx(talkFx, 0);
				}
			}
			else
			{
				dialogeOn = false;
			}
			if((ThereIsEnemy() && !app->sceneBattle->enemy1Dead && !app->sceneBattle->enemy2Dead && !app->sceneBattle->enemy3Dead)|| (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN))
			{
				app->sceneBattle->battleOn = true;

				app->fadeToBlack->FadeToBlk(app->scene, app->sceneBattle, 30);
			}

						
		}
		if (app->scene->currentScene == GameScene::SCENE_TOWN || app->sceneDungeon->currentScene == DungeonScene::SCENE_HALL || app->sceneDungeon->currentScene == DungeonScene::SCENE_MID)
		{
			CheckDoor();
	
		}
		if (app->scene->currentScene == GameScene::SCENE_BSMITH ||app->scene->currentScene == GameScene::SCENE_HOUSE1 || app->scene->currentScene == GameScene::SCENE_INN || app->scene->currentScene == GameScene::SCENE_ENTRYDUNGEON)
		{
			CheckHouseDoor();
			
		}

		if (app->scene->currentScene == GameScene::SCENE_HOUSE1 && (ThereIsNPCUp() || ThereIsNPCBelow() || ThereIsNPCLeft() || ThereIsNPCRight()))
		{
			app->quests->quest1Taken = true;
		}
		if (app->scene->currentScene == GameScene::SCENE_INN && (ThereIsNPCUp() || ThereIsNPCBelow() || ThereIsNPCLeft() || ThereIsNPCRight()))
		{
			app->quests->quest2Taken = true;
		}
		if (app->scene->currentScene == GameScene::SCENE_BSMITH && (ThereIsNPCUp() || ThereIsNPCBelow() || ThereIsNPCLeft() || ThereIsNPCRight()))
		{
			app->quests->quest3Taken = true;
		}

		if (shotCountdown > 0) --shotCountdown;
		
		if (DungeonDoorOpen() && app->sceneDungeon->currentScene == DungeonScene::SCENE_HALL && !button2Done)
		{
			app->map->puzzle1DungeonDone = true;
			
			app->audio->PlayFx(buttonFx, 0);
			app->audio->PlayFx(puzzleSolvedFx, 0);
			button2Done = true;
		}		
		if (DungeonFloorUp() && app->sceneDungeon->currentScene == DungeonScene::SCENE_HALL && !button1Done)
		{
			app->map->buttonFloorPressed = true;
			app->audio->PlayFx(buttonFx, 0);
			button1Done = true;
		}

		if (OpenChest() && app->sceneDungeon->currentScene == DungeonScene::SCENE_HALL)
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				//app->audio->PlayFx(chestFx, 0);
				app->map->chestOpened = true;
			}
		}
		if (OpenLever1() && app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && !lever1Done)
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				app->audio->PlayFx(leverFx, 0);
				app->map->lever1Pressed = true;
				spikes1Down  =  true;
				spikes2Down = false;
				spikes3Down = false;
				lever1Done = true;
			}
		}
		if (OpenLever2() && app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && !lever2Done)
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				app->audio->PlayFx(leverFx, 0);
				app->map->lever2Pressed = true;
				spikes1Down = false;
				spikes2Down = true;
				spikes3Down = false;
				lever2Done = true;
			}
		}
		if (OpenLever3() && app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && !lever3Done)
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				app->audio->PlayFx(leverFx, 0);
				app->map->lever3Pressed = true;
				spikes1Down = true;
				spikes2Down = true;
				spikes3Down = false;
				lever3Done = true;
			}
		}
		if (OpenLeverFinal() && app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && !leverFinalDone)
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				if (app->map->lever3Pressed && app->map->lever2Pressed && app->map->lever1Pressed)
				{
					app->audio->PlayFx(leverFx, 0);
					app->audio->PlayFx(puzzleSolvedFx, 0);
					app->map->leverFinalPressed = true;
					spikes1Down = true;
					spikes2Down = true;
					spikes3Down = true;
					leverFinalDone = true;
				}else app->audio->PlayFx(leverErrorFx, 0);
			}
		}


		if (TakeBlueKey() && app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && !keyBlueDone)
		{
			app->map->blueKeyTaken = true;
			app->audio->PlayFx(keyTakenFx, 0);
			keyBlueDone = true;
		}
		if (TakeRedKey() && app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && !keyRedDone)
		{
			app->map->redKeyTaken = true;
			app->audio->PlayFx(keyTakenFx, 0);
			keyRedDone = true;
		}
		if (TakeYellowKey() && app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && !keyYellowDone)
		{
			app->map->yellowKeyTaken = true;
			app->audio->PlayFx(keyTakenFx, 0);
			keyYellowDone = true;
		}
		if (TakeGreenKey() && app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && !keyGreenDone)
		{
			app->map->greenKeyTaken = true;
			app->audio->PlayFx(keyTakenFx, 0);
			app->audio->PlayFx(puzzleSolvedFx, 0);
			keyGreenDone = true;
		}
	}

	//restart when dies
	if (spiked && !dead)
	{
		currentAnimation = &deathAnim;
		if (deathAnim.HasFinished())
		{
			app->render->RestartValues();
		}
	}
	
	if (dead)
	{
		currentAnimation = &deathAnim;
		if (deathAnim.HasFinished())
		{
			app->fadeToBlack->FadeToBlk(app->scene, app->sceneLose, 30);
			app->render->RestartValues();
		}
	}



	//Dumbledore (aixo es posara a true al dialeg del mago pop)
	//if (!mageTkn&& position.y >= 319) mageTkn = true;
	//
	//if (!knightTkn&& position.y <= 150) knightTkn = true;
	if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) app->scene->mageTkn = true;

	if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN) app->scene->knightTkn = true;

	if (battleDoneMage && !app->sceneBattle->mageRevive) app->scene->mageTkn = true;
	if (battleDoneKnight && !app->sceneBattle->knightRevive) app->scene->knightTkn = true;
	


	currentAnimation->Update();
	return true;
}

bool Player::PostUpdate()
{
	if ((this->active == true) && (!app->scene->paused) && (!app->sceneDungeon->paused) && !app->sceneBattle->playerDead)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texPlayer, position.x, position.y, &rect);
	}
	return true;
}

void Player::CheckDoor()
{
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		door;

		while (layer != NULL)
		{
			if (layer->data->name == "colliders" && layer != NULL)
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + i * 4, position.y);
					door = layer->data->Get(tilePosition.x, tilePosition.y);
				}
			}
			layer = layer->next;
		}
	}

}

void Player::CheckHouseDoor()
{
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		houseDoor;

		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + i * 4, position.y + playerHeight / 2);
					houseDoor = layer->data->Get(tilePosition.x, tilePosition.y);
				}
			}
			layer = layer->next;
		}
	}

}


void Player::addItemToIventory(Item * item)
{

	for (int i = 0; i < INVENTORY_SIZE; ++i)
	{

		if (inventory[i] == nullptr)
		{
			coins -= item->price;
			inventory[i] = item;
			break;
		}

	}

}


void Player::drawPlayerInventory()
{
	SDL_Rect screen;
	screen.x = 0;
	screen.y = 0;
	screen.w = 1280;
	screen.h = 720;

	if (checkInventoryPos(0))
	{
		
		SDL_Rect item;
		item.x = 345;
		item.y = 140;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[0]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[0]->texture, 115, 47, NULL);

	}

	if (checkInventoryPos(1))
	{
		SDL_Rect item;
		item.x = 471;
		item.y = 140;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[1]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[1]->texture, 157, 47, NULL);

	}

	if (checkInventoryPos(2))
	{
		SDL_Rect item;
		item.x = 597;
		item.y = 140;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[2]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[2]->texture, 199, 47, NULL);


	}

	if (checkInventoryPos(3))
	{
		SDL_Rect item;
		item.x = 723;
		item.y = 140;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[3]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[3]->texture, 241, 47, NULL);

	}

	if (checkInventoryPos(4))
	{
		SDL_Rect item;
		item.x = 849;
		item.y = 140;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[4]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[4]->texture, 283, 47, NULL);

	}

	if (checkInventoryPos(5))
	{

		SDL_Rect item;
		item.x = 345;
		item.y = 269;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[5]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[5]->texture, 115, 89, NULL);

	}

	if (checkInventoryPos(6))
	{
		SDL_Rect item;
		item.x = 471;
		item.y = 269;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[6]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[6]->texture, 157, 89, NULL);

	}

	if (checkInventoryPos(7))
	{
		SDL_Rect item;
		item.x = 597;
		item.y = 269;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[7]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[7]->texture, 199, 89, NULL);

	}

	if (checkInventoryPos(8))
	{
		SDL_Rect item;
		item.x = 723;
		item.y = 269;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[8]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[8]->texture, 241, 89, NULL);

	}

	if (checkInventoryPos(9))
	{
		SDL_Rect item;
		item.x = 849;
		item.y = 269;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[9]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[9]->texture, 283, 89, NULL);

	}

	if (checkInventoryPos(10))
	{

		SDL_Rect item;
		item.x = 471;
		item.y = 398;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[10]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[10]->texture, 157, 131, NULL);

	}

	if (checkInventoryPos(11))
	{

		SDL_Rect item;
		item.x = 597;
		item.y = 398;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[11]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[11]->texture, 199, 131, NULL);

	}

	if (checkInventoryPos(12))
	{

		SDL_Rect item;
		item.x = 723;
		item.y = 398;
		item.w = 102;
		item.h = 102;
		SDL_RenderCopy(app->render->renderer, inventory[12]->texture, &screen, &item);
		//app->render->DrawTexture(inventory[12]->texture, 241, 131, NULL);

	}


}


bool Player::checkInventoryPos(int pos)
{

	return inventory[pos] != nullptr;

}

bool Player::isItemInInventory(ItemType itemtype)
{
	for (int i = 0; i < INVENTORY_SIZE; ++i)
	{
		if (inventory[i] != nullptr)
			if (inventory[i]->type == itemtype)
				return true;

	}


	return false;
}

bool Player::ThereIsTopWall()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int groundId;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + i * 4, position.y-1);
					groundId = layer->data->Get(tilePosition.x, tilePosition.y);

					if (groundId == COLLIDER_RED || groundId == COLLIDER_RED_HOUSE 
						|| groundId == COLLIDER_RED_FOREST || groundId == COLLIDER_RED_DUNGEON || (groundId == COLLIDER_BLACK_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_HALL)
						|| (groundId == COLLIDER_BLACK_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_MID)
						|| (groundId == COLLIDER_CIAN_DUNGEON && !app->map->puzzle1DungeonDone && app->sceneDungeon->currentScene == DungeonScene::SCENE_HALL) 
						|| (groundId == COLLIDER_ORANGE_DUNGEON && !app->map->buttonFloorPressed)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && groundId == COLLIDER_BLUE_DUNGEON) 
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && groundId == COLLIDER_ORANGE_DUNGEON) 
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && groundId == COLLIDER_YELLOW_DUNGEON)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && groundId == COLLIDER_BEIGE_DUNGEON && !app->map->redKeyTaken)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && groundId == COLLIDER_BROWN_DUNGEON && !app->map->blueKeyTaken)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && groundId == COLLIDER_GREENER_DUNGEON && !app->map->greenKeyTaken))
					{
						valid = true;
					}
				}

			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::ThereIsBottomWall()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int groundId;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + i * 4, position.y + playerHeight);
					groundId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (groundId == COLLIDER_RED || groundId == COLLIDER_RED_HOUSE || groundId == COLLIDER_RED_FOREST || groundId == COLLIDER_RED_DUNGEON 
						|| (groundId == COLLIDER_BLACK_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_HALL)
						|| (groundId == COLLIDER_BLACK_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_MID)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && groundId == COLLIDER_BLUE_DUNGEON) 
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && groundId == COLLIDER_ORANGE_DUNGEON)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && groundId == COLLIDER_YELLOW_DUNGEON)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && groundId == COLLIDER_YELLOW_DUNGEON && !app->map->yellowKeyTaken)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && groundId == COLLIDER_GREY_DUNGEON && !app->map->blueKeyTaken))
					{
						valid = true;
					}
				}

			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::ThereIsLeftWall()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int leftWallId;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x-1, position.y + i * 4);
					leftWallId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (leftWallId == COLLIDER_RED || leftWallId == COLLIDER_RED_HOUSE || leftWallId == COLLIDER_RED_FOREST 
						|| leftWallId == COLLIDER_RED_DUNGEON || (leftWallId == COLLIDER_BLACK_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_HALL)
						|| (leftWallId == COLLIDER_BLACK_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_MID)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && leftWallId == COLLIDER_BLUE_DUNGEON)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && leftWallId == COLLIDER_ORANGE_DUNGEON)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && leftWallId == COLLIDER_YELLOW_DUNGEON) 
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && leftWallId == COLLIDER_BEIGE_DUNGEON && !spikes1Down)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && leftWallId == COLLIDER_BROWN_DUNGEON && !spikes2Down)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && leftWallId == COLLIDER_CIAN_DUNGEON && !spikes3Down)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && leftWallId == COLLIDER_YELLOW_DUNGEON && !app->map->yellowKeyTaken)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && leftWallId == COLLIDER_GREY_DUNGEON && !app->map->blueKeyTaken))
					{
						valid = true;
					}
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::ThereIsRightWall()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int rightWallId;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + playerWidth+1, position.y + i * 4);
					rightWallId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (rightWallId == COLLIDER_RED || rightWallId == COLLIDER_RED_HOUSE || rightWallId == COLLIDER_RED_FOREST || rightWallId == COLLIDER_RED_DUNGEON 
						|| (rightWallId == COLLIDER_BLACK_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_HALL)
						|| (rightWallId == COLLIDER_BLACK_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_MID)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && rightWallId == COLLIDER_BLUE_DUNGEON)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && rightWallId == COLLIDER_ORANGE_DUNGEON)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && rightWallId == COLLIDER_YELLOW_DUNGEON)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && rightWallId == COLLIDER_BEIGE_DUNGEON && !spikes1Down)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && rightWallId == COLLIDER_BROWN_DUNGEON && !spikes2Down)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_MID && rightWallId == COLLIDER_CIAN_DUNGEON && !spikes3Down)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && rightWallId == COLLIDER_YELLOW_DUNGEON && !app->map->yellowKeyTaken)
						|| (app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS && rightWallId == COLLIDER_GREY_DUNGEON && !app->map->blueKeyTaken))
					{
						valid = true;
					}
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::ThereIsLimit()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int limitId;
		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + i * 4, position.y);
					limitId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (limitId == COLLIDER_BEIGE) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::ThereIsHouseClosed()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int limitId;
		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + i * 4, position.y);
					limitId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (limitId == COLLIDER_ORANGE) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

int Player::ThereIsNPC()
{
	bool valid = false;
	bool positionX = false;
	bool positionY = false;
	bool positionX2 = false;
	bool positionY2 = false;
	int npc = 0;
	if (!godModeEnabled && app->scene->currentScene == GameScene::SCENE_TOWN)
	{
		for (int i = -32; i < 32; ++i)
		{
			if (app->scene->npc1->position.x + i == position.x) positionX = true;
		}
		for (int i = -32; i < 32; ++i)
		{
			if (app->scene->npc1->position.y + i == position.y) positionY = true;
		}
	}
	if (positionX && positionY) npc = 1;
	if (positionX2 && positionY2) npc = 4;

	return npc;
}

bool Player::ThereIsNPCBelow()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int groundId;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + i * 4, position.y + playerHeight);
					groundId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (groundId == COLLIDER_CIAN || groundId == COLLIDER_CIAN_HOUSE) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::ThereIsNPCUp()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int rightWallId;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + i * 4, position.y - 1);
					rightWallId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (rightWallId == COLLIDER_CIAN || rightWallId == COLLIDER_CIAN_HOUSE) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::ThereIsNPCLeft()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int leftWallId;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x - 1, position.y + i * 4);
					leftWallId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (leftWallId == COLLIDER_CIAN || leftWallId == COLLIDER_CIAN_HOUSE) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::ThereIsNPCRight()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int rightWallId;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + playerWidth + 1, position.y + i * 4);
					rightWallId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (rightWallId == COLLIDER_CIAN || rightWallId == COLLIDER_CIAN_HOUSE) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::ThereIsEnemy()
{
	
	bool valid = false;
	bool positionX = false;
	bool positionY = false;

	if (!godModeEnabled)
	{
		for (int i = -16; i < 16; ++i)
		{
			if (app->scene->enemy1->position.x + i == position.x) positionX = true;
		}
		for (int i = -16; i < 16; ++i)
		{
			if (app->scene->enemy1->position.y + i == position.y) positionY = true;
		}
	}
	if (positionX && positionY) valid = true;

	return valid;
}

bool Player::ThereIsFlyingEnemy()
{
	bool valid = false;
	bool positionX = false;
	bool positionY = false;

	if (app->scene->flyingEnemy != nullptr)
	{
		if (!godModeEnabled && !app->scene->flyingEnemy->dead)
		{
			for (int i = 0; i < 50; ++i)
			{
				for (int j = 0; j < 30; ++j)
				{
					if (app->scene->flyingEnemy->position.x + 6 + i == position.x + 16 + j) positionX = true;
				}
			}
			for (int i = 0; i < 42; ++i)
			{
				for (int j = 0; j < 62; ++j)
				{
					if (app->scene->flyingEnemy->position.y + 4 + i == position.y + 22 + j) positionY = true;
				}
			}
		}
	}
	if (positionX && positionY) valid = true;

	return valid;
}

bool Player::DungeonDoorOpen()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int doorDungeon;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + i * 4, position.y + playerHeight/2);
				doorDungeon = layer->data->Get(tilePosition.x, tilePosition.y);
				if (doorDungeon == COLLIDER_BLUE_DUNGEON)
				{
					//app->map->checkpointTaken = true;
					valid = true;
				}
			}
		}
		layer = layer->next;
	}
	return valid;

}

bool Player::DungeonFloorUp()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int doorDungeon;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + i * 4, position.y + playerHeight / 2);
				doorDungeon = layer->data->Get(tilePosition.x, tilePosition.y);
				if (doorDungeon == COLLIDER_YELLOW_DUNGEON)
				{
					//app->map->checkpointTaken = true;
					valid = true;
				}
			}
		}
		layer = layer->next;
	}
	
	return valid;

}

bool Player::OpenChest()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int chestId;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + playerWidth + 1, position.y + i * 4);
					chestId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (chestId == COLLIDER_BLACK_DUNGEON) valid = true;
				}
				for (int j = 0; j < 4; ++j)
				{
					tilePosition = app->map->WorldToMap(position.x - 1, position.y + j * 4);
					chestId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (chestId == COLLIDER_BLACK_DUNGEON) valid = true;
				}
				for (int k = 0; k < 4; ++k)
				{
					tilePosition = app->map->WorldToMap(position.x + k * 4, position.y - 1);
					chestId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (chestId == COLLIDER_BLACK_DUNGEON) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::OpenLever1()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int lever1Id;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + playerWidth + 1, position.y + i * 4);
					lever1Id = layer->data->Get(tilePosition.x, tilePosition.y);
					if (lever1Id == COLLIDER_BLUE_DUNGEON) valid = true;
				}
				for (int j = 0; j < 4; ++j)
				{
					tilePosition = app->map->WorldToMap(position.x - 1, position.y + j * 4);
					lever1Id = layer->data->Get(tilePosition.x, tilePosition.y);
					if (lever1Id == COLLIDER_BLUE_DUNGEON) valid = true;
				}
				for (int k = 0; k < 4; ++k)
				{
					tilePosition = app->map->WorldToMap(position.x + k * 4, position.y - 1);
					lever1Id = layer->data->Get(tilePosition.x, tilePosition.y);
					if (lever1Id == COLLIDER_BLUE_DUNGEON) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::OpenLever2()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int lever2Id;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + playerWidth + 1, position.y + i * 4);
					lever2Id = layer->data->Get(tilePosition.x, tilePosition.y);
					if (lever2Id == COLLIDER_ORANGE_DUNGEON) valid = true;
				}
				for (int j = 0; j < 4; ++j)
				{
					tilePosition = app->map->WorldToMap(position.x - 1, position.y + j * 4);
					lever2Id = layer->data->Get(tilePosition.x, tilePosition.y);
					if (lever2Id == COLLIDER_ORANGE_DUNGEON) valid = true;
				}
				for (int k = 0; k < 4; ++k)
				{
					tilePosition = app->map->WorldToMap(position.x + k * 4, position.y - 1);
					lever2Id = layer->data->Get(tilePosition.x, tilePosition.y);
					if (lever2Id == COLLIDER_ORANGE_DUNGEON) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::OpenLever3()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int lever3Id;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + playerWidth + 1, position.y + i * 4);
					lever3Id = layer->data->Get(tilePosition.x, tilePosition.y);
					if (lever3Id == COLLIDER_BLACK_DUNGEON) valid = true;
				}
				for (int j = 0; j < 4; ++j)
				{
					tilePosition = app->map->WorldToMap(position.x - 1, position.y + j * 4);
					lever3Id = layer->data->Get(tilePosition.x, tilePosition.y);
					if (lever3Id == COLLIDER_BLACK_DUNGEON) valid = true;
				}
				for (int k = 0; k < 4; ++k)
				{
					tilePosition = app->map->WorldToMap(position.x + k * 4, position.y - 1);
					lever3Id = layer->data->Get(tilePosition.x, tilePosition.y);
					if (lever3Id == COLLIDER_BLACK_DUNGEON) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::OpenLeverFinal()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int leverFinalId;
		while (layer != NULL)
		{
			if (layer->data->name == "colliders")
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + playerWidth + 1, position.y + i * 4);
					leverFinalId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (leverFinalId == COLLIDER_YELLOW_DUNGEON) valid = true;
				}
				for (int j = 0; j < 4; ++j)
				{
					tilePosition = app->map->WorldToMap(position.x - 1, position.y + j * 4);
					leverFinalId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (leverFinalId == COLLIDER_YELLOW_DUNGEON) valid = true;
				}
				for (int k = 0; k < 4; ++k)
				{
					tilePosition = app->map->WorldToMap(position.x + k * 4, position.y - 1);
					leverFinalId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (leverFinalId == COLLIDER_YELLOW_DUNGEON) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;

}


bool Player::TakeBlueKey()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int blueKey;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + i * 4, position.y + playerHeight / 2);
				blueKey = layer->data->Get(tilePosition.x, tilePosition.y);
				if (blueKey == COLLIDER_PINK_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS)
				{
					//app->map->checkpointTaken = true;
					valid = true;
				}
			}
		}
		layer = layer->next;
	}
	return valid;

}

bool Player::TakeRedKey()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int redKey;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + i * 4, position.y + playerHeight / 2);
				redKey = layer->data->Get(tilePosition.x, tilePosition.y);
				if (redKey == COLLIDER_BLACK_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS)
				{
					//app->map->checkpointTaken = true;
					valid = true;
				}
			}
		}
		layer = layer->next;
	}
	return valid;

}

bool Player::TakeGreenKey()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int greenKey;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + i * 4, position.y + playerHeight / 2);
				greenKey = layer->data->Get(tilePosition.x, tilePosition.y);
				if (greenKey == COLLIDER_BLUE_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS)
				{
					//app->map->checkpointTaken = true;
					valid = true;
				}
			}
		}
		layer = layer->next;
	}
	return valid;

}

bool Player::TakeYellowKey()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int yellowKey;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + i * 4, position.y + playerHeight / 2);
				yellowKey = layer->data->Get(tilePosition.x, tilePosition.y);
				if (yellowKey == COLLIDER_CIAN_DUNGEON && app->sceneDungeon->currentScene == DungeonScene::SCENE_BOSS)
				{
					//app->map->checkpointTaken = true;
					valid = true;
				}
			}
		}
		layer = layer->next;
	}
	return valid;

}


bool Player::ThereIsDoor()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int groundId;

		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x, position.y + 21 + i * 16);
					groundId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (groundId == COLLIDER_GREEN) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::LoseLifes()
{
	lifes--;
	if (lifes == 0) dead = true;
	
	app->audio->PlayFx(playerDeathFx, 0);

	spiked = true;

	return true;
}

bool Player::CleanUp()
{
	LOG("Freeing scene");
	bool ret = true;

	if (texPlayer != nullptr)
	{
		ret = app->tex->UnLoad(texPlayer);
		texPlayer = nullptr;
	}
	return ret;
}