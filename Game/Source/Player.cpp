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


#define COLLIDER_GREEN 265
#define COLLIDER_RED 266
#define COLLIDER_BLUE 267
#define COLLIDER_YELLOW 268
#define COLLIDER_PINK 269
#define COLLIDER_GREY 270
#define COLLIDER_ORANGE 271

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");
	
	if (app->sceneIntro->playClicked)
	{
		position.x = 350;
		position.y = 875;
		app->sceneIntro->playClicked = false;
		app->sceneIntro->posContinue = false;
		app->sceneWin->won = false;
		app->sceneLose->lost = false;
	}
	else if (!app->sceneIntro->posContinue)
	{
		position.x = 350;
		position.y = 875;

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
	idlAnim.PushBack({ 0, 0, 64, 85 });
	idlAnim.PushBack({ 0, 681, 64, 85 });
	idlAnim.speed = 0.02f;

	//move right
	rightAnim.PushBack({ 0, 85, 64, 85 });
	rightAnim.PushBack({ 0, 170, 64, 85 });
	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 0, 255, 64, 85 });
	leftAnim.PushBack({ 0, 340, 64, 85 });
	leftAnim.speed = 0.1f;

	//jumpAnimRight
	jumpAnimRight.PushBack({ 0, 425, 64, 85 });
	jumpAnimRight.speed = 0.1f;
	
	//jumpAnimRight
	jumpAnimLeft.PushBack({ 0, 510, 64, 85 });
	jumpAnimLeft.speed = 0.1f;

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
		texPlayer = app->tex->Load("Assets/Textures/player_textures.png");
		texFireBall = app->tex->Load("Assets/Textures/shot_fireball.png");
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

		if (ThereIsGround()) speedY = 0;


		if ((ThereAreSpikes() || ThereIsEnemy() || ThereIsFlyingEnemy()))
		{
			LoseLifes();
		}

		if (ThereIsDoor() && app->map->keyTaken) win = true;
		else
		{
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && !ThereAreSpikes())
			{
				if (!ThereIsLeftWall() && !ThereIsChestLeft())
				{
					position.y -= speedX;
					currentAnimation = &leftAnim;
				}				
			}
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !ThereAreSpikes())
			{
				if (!ThereIsLeftWall() && !ThereIsChestLeft())
				{
					position.y += speedX;
					currentAnimation = &leftAnim;
				}				
			}

			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !ThereAreSpikes())
			{
				if (!ThereIsLeftWall() && !ThereIsChestLeft())
				{
					position.x -= speedX;
					currentAnimation = &leftAnim;
				}
			}
			else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !ThereAreSpikes())
			{
				if (!ThereIsRightWall() && !ThereIsChestRight())
				{
					position.x += speedX;
					currentAnimation = &rightAnim;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (ThereIsGround() || ThereIsChestBelow()) && !ThereAreSpikes())
			{
				isJumping = true;
				speedY = 5.0f;
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

		if (shotCountdown > 0) --shotCountdown;

		if (ThereIsChestBelow() || ThereIsChestLeft() || ThereIsChestRight())
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && app->map->puzzleTaken)
			{
				app->map->chestTaken = true;
				app->audio->PlayFx(chestFx, 0);

			}
		}

		if (TakeKey())
		{
			app->map->keyTaken = true;
			if (counterKey == 0) app->audio->PlayFx(itemTakenFx, 0);
			counterKey = 1;
		}
		if (TakePuzzle())
		{
			app->map->puzzleTaken = true;
			if (counterPuzzle == 0) app->audio->PlayFx(itemTakenFx, 0);
			counterPuzzle = 1;
		}
		if (TakeCheckpoint())
		{
			app->map->checkpointTaken = true;
			if (counterCheckpoint == 0) app->audio->PlayFx(checkpointFx, 0);
			counterCheckpoint = 1;
		}
		if (TakeHeart() && app->map->chestTaken)
		{
			app->map->heartTaken = true;
			if (counterHeart == 0)
			{
				lifes++;
				app->audio->PlayFx(heartFx, 0);
			}
			counterHeart = 1;
		}
	}
	//restart when dies
	if (spiked && !dead)
	{
		currentAnimation = &deathAnim;
		if (deathAnim.HasFinished())
		{
			app->render->RestartValues();
			//app->enemy->Enable();
			//app->flyingEnemy->Enable();
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

bool Player::ThereIsGround()
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
				for (int i = 0; i < 5; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + 24 + i * 4, position.y + playerHeight);
					groundId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (groundId == COLLIDER_RED) valid = true;
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
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x, position.y + 21 + i * 16);
					leftWallId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (leftWallId == COLLIDER_RED) valid = true;
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
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 4; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + playerWidth, position.y + 21 + i * 16);
					rightWallId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (rightWallId == COLLIDER_RED) valid = true;
				}
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::ThereIsChestBelow()
{
	bool valid = false;
	if (!app->map->chestTaken)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int groundId;
		while (layer != NULL)
		{
			for (int i = 0; i < 5; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + 24 + i * 4, position.y + playerHeight);
				groundId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (groundId == COLLIDER_GREY) valid = true;
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::ThereIsChestLeft()
{
	bool valid = false;
	if (!app->map->chestTaken)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int leftWallId;
		while (layer != NULL)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x, position.y + 21 + i * 16);
				leftWallId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (leftWallId == COLLIDER_GREY) valid = true;
			}
			layer = layer->next;
		}
	}
	return valid;

}

bool Player::ThereIsChestRight()
{
	bool valid = false;
	if (!app->map->chestTaken)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int rightWallId;
		while (layer != NULL)
		{
			for (int i = 0; i < 4; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + playerWidth, position.y + 21 + i * 16);
				rightWallId = layer->data->Get(tilePosition.x, tilePosition.y);
				if (rightWallId == COLLIDER_GREY) valid = true;
			}
			layer = layer->next;
		}
	}
	return valid;
}

bool Player::ThereAreSpikes()
{
	bool valid = false;
	if (!godModeEnabled)
	{
		iPoint tilePosition;
		ListItem<MapLayer*>* layer = app->map->data.layers.start;
		int spikesId;
		while (layer != NULL)
		{
			if (layer->data->properties.GetProperty("Navigation") == 0)
			{
				for (int i = 0; i < 3; ++i)
				{
					tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + 21);
					spikesId = layer->data->Get(tilePosition.x, tilePosition.y);
					if (spikesId == COLLIDER_YELLOW) valid = true;
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

bool Player::TakeKey()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int key;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 3; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + 21);
				key = layer->data->Get(tilePosition.x, tilePosition.y);
				if (key == COLLIDER_BLUE) valid = true;
			}

		}
		layer = layer->next;
	}
	return valid;

}

bool Player::TakePuzzle()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int puzzle;
	while (layer != NULL)
	{
		for (int i = 0; i < 3; ++i)
		{
			tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + 21);
			puzzle = layer->data->Get(tilePosition.x, tilePosition.y);
			if (puzzle == COLLIDER_ORANGE) valid = true;
		}
		layer = layer->next;
	}
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

bool Player::TakeHeart()
{
	bool valid = false;
	iPoint tilePosition;
	ListItem<MapLayer*>* layer = app->map->data.layers.start;
	int heart;
	while (layer != NULL)
	{
		if (layer->data->properties.GetProperty("Navigation") == 0)
		{
			for (int i = 0; i < 3; ++i)
			{
				tilePosition = app->map->WorldToMap(position.x + 19 + i * 13, position.y + 21);
				heart = layer->data->Get(tilePosition.x, tilePosition.y);
				if (heart == COLLIDER_GREY) valid = true;
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