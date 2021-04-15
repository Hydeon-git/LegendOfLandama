#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Scene.h"
#include "SceneLose.h"
#include "Enemy.h"
#include "ModuleParticles.h"
#include "FlyingEnemy.h"
#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"
#include "SceneIntro.h"
#include "SceneWin.h"



Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");
	
	if (app->sceneIntro->startClicked)
	{
		position.x = 200;
		position.y = 100;
		app->sceneIntro->startClicked = false;
		app->sceneIntro->posContinue = false;
		app->sceneWin->won = false;
		app->sceneLose->lost = false;
	}
	else if (!app->sceneIntro->posContinue)
	{
		position.x = 200;
		position.y = 100;

	}
	else if (app->sceneWin->won||app->sceneLose->lost)
	{
		position.x = 350;
		position.y = 875;
		app->sceneWin->won = false;
		app->sceneLose->lost = false;
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
		dead = false;
		spiked = false;
		win = false;

		if (!app->sceneIntro->posContinue)
		{
			app->map->keyTaken = false;
			app->map->chestTaken = false;
			app->map->heartTaken = false;
			app->map->puzzleTaken = false;
			app->map->checkpointTaken = false;
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
		texPlayer = app->tex->Load("Assets/Textures/main_character.png");
		playerDeathFx = app->audio->LoadFx("Assets/Audio/Fx/death_sound.wav");
		itemTakenFx = app->audio->LoadFx("Assets/Audio/Fx/item.wav");
		checkpointFx = app->audio->LoadFx("Assets/Audio/Fx/checkpoint.wav");
		chestFx = app->audio->LoadFx("Assets/Audio/Fx/chest.wav");
		heartFx = app->audio->LoadFx("Assets/Audio/Fx/heart.wav");
		fireFx = app->audio->LoadFx("Assets/Audio/Fx/fire.wav");
		currentAnimation = &idlAnim;
	}


	return true;
}

bool Player::Update(float dt)
{
	if (app->scene->player->loaded)
	{
		app->render->camera.x = -588;
		app->render->camera.y = -99;
		loaded = false;
	}
	if (!spiked && !dead)
	{
		currentAnimation = &idlAnim;

		if (ThereIsDoor() && app->map->keyTaken) win = true;
		else
		{
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			{
				if (!ThereIsTopWall())
				{
					position.y -= speed;
					currentAnimation = &upAnim;
				}				
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			{
				if (!ThereIsBottomWall())
				{
					position.y += speed;
					currentAnimation = &leftAnim;
				}				
			}

			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))
			{
				if (!ThereIsLeftWall())
				{
					position.x -= speed;
					currentAnimation = &leftAnim;
				}
			}
			else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
			{
				if (!ThereIsRightWall())
				{
					position.x += speed;
					currentAnimation = &rightAnim;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
			{
				if (currentAnimation == &leftAnim)
				{
					if (shotCountdown == 0)
					{
						Particle* newParticle = app->scene->particles->AddParticle(app->scene->particles->fireBallLeft, position.x, position.y + 50);
						app->audio->PlayFx(fireFx, 0);
						shotCountdown = shotMaxCountdown;
					}
				}
				else
				{
					if (shotCountdown == 0)
					{
						Particle* newParticle = app->scene->particles->AddParticle(app->scene->particles->fireBallRight, position.x + 50, position.y + 50);
						app->audio->PlayFx(fireFx, 0);
						shotCountdown = shotMaxCountdown;
					}
				}				
			}
		}
		if (app->scene->currentScene == GameScene::SCENE_TOWN)
		{
			CheckDoor();
		}
		if (app->scene->currentScene == GameScene::SCENE_BSMITH ||app->scene->currentScene == GameScene::SCENE_HOUSE1 ||app->scene->currentScene == GameScene::SCENE_INN)
		{
			CheckHouseDoor();
		}
		

		if (shotCountdown > 0) --shotCountdown;

			
		if (TakeCheckpoint())
		{
			app->map->checkpointTaken = true;
			if (counterCheckpoint == 0) app->audio->PlayFx(checkpointFx, 0);
			counterCheckpoint = 1;
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

	currentAnimation->Update();
	return true;
}

bool Player::PostUpdate()
{
	if (this->active == true)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texPlayer, position.x, position.y, &rect);
	}
	return true;
}

void Player::CheckDoor()
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

void Player::CheckHouseDoor()
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
				tilePosition = app->map->WorldToMap(position.x + i * 4, position.y);
				houseDoor = layer->data->Get(tilePosition.x, tilePosition.y);
			}
		}
		layer = layer->next;
	}
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
					if (groundId == COLLIDER_RED || groundId == COLLIDER_RED_HOUSE) valid = true;
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
					if (groundId == COLLIDER_RED || groundId == COLLIDER_RED_HOUSE) valid = true;
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
					if (leftWallId == COLLIDER_RED || leftWallId == COLLIDER_RED_HOUSE) valid = true;
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
					if (rightWallId == COLLIDER_RED || rightWallId == COLLIDER_RED_HOUSE) valid = true;
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
					if (limitId == COLLIDER_PINK) valid = true;
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

bool Player::ThereIsEnemy()
{
	
	bool valid = false;
	bool positionX = false;
	bool positionY = false;

	if (!godModeEnabled && !app->scene->enemy->dead)
	{
		for (int i = 0; i < 30; ++i)
		{
			for (int j = 0; j < 30; ++j)
			{
				if (app->scene->enemy->position.x + 16 + i == position.x + 16 + j) positionX = true;
			}
		}
		for (int i = 0; i < 62; ++i)
		{
			for (int j = 0; j < 62; ++j)
			{
				if (app->scene->enemy->position.y + 22 + i == position.y + 22 + j) positionY = true;
			}
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

bool Player::TakeCheckpoint()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int checkpoint;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 3; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + 21);
				checkpoint = layer->data->Get(tilePosition.x, tilePosition.y);
				if (checkpoint == COLLIDER_PINK)
				{
					app->SaveGameRequest();
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
	app->tex->UnLoad(texPlayer);
	app->tex->UnLoad(texFireBall);
	//app->entityManager->DestroyEntity(this);

	return true;
}