#include <string>
#include "SceneBattle.h"
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
#include "FadeToBlack.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "Font.h"
#include "Map.h"
#include "EntityManager.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "NPC5.h"
#include "NPC7.h"

#include "Defs.h"
#include "Log.h"
#include <Time.h>


SceneBattle::SceneBattle() : Module()
{
	name.Create("SceneBattle");
}

SceneBattle::~SceneBattle()
{

}

bool SceneBattle::Awake(pugi::xml_node& node)
{
	fullscreenRect = new SDL_Rect{ 0, 0, 1280, 720 };
	return true;
}

bool SceneBattle::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		app->entityManager->Enable();
		app->scene->player->battleDoneMage = true;
		app->scene->player->battleDoneKnight = true;
		if (app->scene->knightTkn)
		{
			LOG("Loading background assets");

			player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
			npc5 = (NPC5*)app->entityManager->CreateEntity(EntityType::NPC5);
			npc7 = (NPC7*)app->entityManager->CreateEntity(EntityType::NPC7);
			enemy1 = (Enemy1*)app->entityManager->CreateEntity(EntityType::Enemy1);
			enemy2 = (Enemy2*)app->entityManager->CreateEntity(EntityType::Enemy2);
			enemy3 = (Enemy3*)app->entityManager->CreateEntity(EntityType::Enemy3);

			player->Start();
			npc5->Start();
			npc7->Start();

			// Enemy Central
			enemy1->Start();
			// Enemy Up
			enemy2->Start();
			// Enemy Down
			enemy3->Start();

			// Counters
			heroineCounter = 1;
			mageCounter = 1;
			knightCounter = 1;

			enemyCounter = 0;

			//introText = app->tex->Load("Assets/Textures/portada.png");
			app->audio->PlayMusic("Assets/Audio/Music/battle_theme.ogg");
			battletext = app->tex->Load("Assets/Textures/battleback.png");
			app->sceneBattle->player->onBattle = true;
			app->sceneLose->Disable();
			app->sceneWin->Disable();
			app->scene->Disable();
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			battleOn = true;
			char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
			whiteFont = app->font->Load("Assets/Textures/white_font_mini.png", lookupTable, 1);
			goldFont = app->font->Load("Assets/Textures/gold_font_mini.png", lookupTable, 1);
			selectorTex = app->tex->Load("Assets/Textures/pointer.png");
			hitEnemyFx = app->audio->LoadFx("Assets/Audio/Fx/hitEnemy.wav");
			magicEnemyFx = app->audio->LoadFx("Assets/Audio/Fx/magic.wav");


			btnHeroine = new GuiButton(1, { 25, 182, 60, 15 }, "Heroine");
			btnHeroine->SetObserver(this);
			btnMage = new GuiButton(2, { 25, 200, 60, 15 }, "Mage");
			btnMage->SetObserver(this);
			btnKnight = new GuiButton(9, { 25, 218, 60, 15 }, "Knight");
			btnKnight->SetObserver(this);
			btnAttack = new GuiButton(3, { 140, 175, 60, 15 }, "Attack");
			btnAttack->SetObserver(this);
			btnMagic = new GuiButton(4, { 140, 195, 60, 15 }, "Magic");
			btnMagic->SetObserver(this);
			btnDefense = new GuiButton(5, { 140, 215, 60, 15 }, "Defense");
			btnDefense->SetObserver(this);

			btnEnemy1 = new GuiButton(6, { 300, 100, 16, 16 }, "");
			btnEnemy1->SetObserver(this);
			btnEnemy2 = new GuiButton(7, { 315, 80, 16, 16 }, "");
			btnEnemy2->SetObserver(this);
			btnEnemy3 = new GuiButton(8, { 315, 120, 16, 16 }, "");
			btnEnemy3->SetObserver(this);
		}
		else // Heroine and Mage vs 3
		{
			LOG("Loading background assets");

			player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
			npc5 = (NPC5*)app->entityManager->CreateEntity(EntityType::NPC5);
			enemy1 = (Enemy1*)app->entityManager->CreateEntity(EntityType::Enemy1);
			enemy2 = (Enemy2*)app->entityManager->CreateEntity(EntityType::Enemy2);
			enemy3 = (Enemy3*)app->entityManager->CreateEntity(EntityType::Enemy3);

			player->Start();
			npc5->Start();

			// Enemy Central
			enemy1->Start();
			// Enemy Up
			enemy2->Start();
			// Enemy Down
			enemy3->Start();

			// Counters
			heroineCounter = 1;
			mageCounter = 1;

			enemyCounter = 0;

			//introText = app->tex->Load("Assets/Textures/portada.png");
			app->audio->PlayMusic("Assets/Audio/Music/battle_theme.ogg");
			battletext = app->tex->Load("Assets/Textures/battleback.png");
			app->sceneBattle->player->onBattle = true;
			app->sceneLose->Disable();
			app->sceneWin->Disable();
			app->scene->Disable();
			app->render->camera.x = 0;
			app->render->camera.y = 0;
			battleOn = true;
			char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
			whiteFont = app->font->Load("Assets/Textures/white_font_mini.png", lookupTable, 1);
			goldFont = app->font->Load("Assets/Textures/gold_font_mini.png", lookupTable, 1);
			selectorTex = app->tex->Load("Assets/Textures/pointer.png");

			hitEnemyFx = app->audio->LoadFx("Assets/Audio/Fx/hitEnemy.wav");
			magicEnemyFx = app->audio->LoadFx("Assets/Audio/Fx/magic.wav");


			btnHeroine = new GuiButton(1, { 25, 200, 60, 15 }, "Heroine");
			btnHeroine->SetObserver(this);
			btnMage = new GuiButton(2, { 25, 220, 60, 15 }, "Mage");
			btnMage->SetObserver(this);
			btnAttack = new GuiButton(3, { 140, 180, 60, 15 }, "Attack");
			btnAttack->SetObserver(this);
			btnMagic = new GuiButton(4, { 140, 200, 60, 15 }, "Magic");
			btnMagic->SetObserver(this);
			btnDefense = new GuiButton(5, { 140, 220, 60, 15 }, "Defense");
			btnDefense->SetObserver(this);

			btnEnemy1 = new GuiButton(6, { 300, 100, 16, 16 }, "");
			btnEnemy1->SetObserver(this);
			btnEnemy2 = new GuiButton(7, { 315, 80, 16, 16 }, "");
			btnEnemy2->SetObserver(this);
			btnEnemy3 = new GuiButton(8, { 315, 120, 16, 16 }, "");
			btnEnemy3->SetObserver(this);
		}



	}
	return ret;
}

bool SceneBattle::Update(float dt)
{
	GamePad& pad = app->input->pads[0];

	if (app->scene->knightTkn)
	{
		if (enemiesAlive == 0 || (playerDead && mageDead && knightDead))
		{
			app->fadeToBlack->FadeToBlk(this, app->scene, 30);
			app->fadeToBlack->animId = 1;
			battleOn = false;
			battleEnd = true;
		}
		btnHeroine->Update(dt);
		btnMage->Update(dt);
		btnKnight->Update(dt);
		if (heroine || mage || knight)
		{
			btnAttack->Update(dt);
			btnMagic->Update(dt);
			btnDefense->Update(dt);
		}
		if (attack || magic)
		{
			btnEnemy1->Update(dt);
			btnEnemy2->Update(dt);
			btnEnemy3->Update(dt);
		}

		//dead
		if (player->playerHealth <= 0)
		{
			player->playerHealth = 0;
			heroineCounter = 0;
			playerDead = true;
			playerRevive = true;

		}
		if (npc5->mageHealth <= 0)
		{
			npc5->mageHealth = 0;
			mageCounter = 0;
			mageDead = true;
			mageRevive = true;
		}
		if (npc7->knightHealth <= 0)
		{
			npc7->knightHealth = 0;
			knightCounter = 0;
			knightDead = true;
			knightRevive = true;
		}


		if (heroineCounter == 0 && mageCounter == 0 && knightCounter == 0)
		{

			enemyCounter++;
			if (enemiesAlive == 3)
			{
				if (enemyCounter == 150)
				{
					EnemyAttack();
				}
				if (enemyCounter == 250)
				{
					EnemyAttack();
				}
				if (enemyCounter == 350)
				{
					EnemyAttack();
					enemyCounter = 0;
					if(!playerDead)	heroineCounter = 1;
					if(!mageDead) mageCounter = 1;
					if(!knightDead) knightCounter = 1;
					btnHeroine->state = GuiControlState::NORMAL;
					btnMage->state = GuiControlState::NORMAL;
					btnKnight->state = GuiControlState::NORMAL;
				}
			}
			else if (enemiesAlive == 2)
			{
				if (enemyCounter == 150)
				{
					EnemyAttack();
				}
				if (enemyCounter == 250)
				{
					EnemyAttack();
					enemyCounter = 0;
					if (!playerDead) heroineCounter = 1;
					if (!mageDead) mageCounter = 1;
					if (!knightDead) knightCounter = 1;
					btnHeroine->state = GuiControlState::NORMAL;
					btnMage->state = GuiControlState::NORMAL;
					btnKnight->state = GuiControlState::NORMAL;
				}
			}
			else if (enemiesAlive == 1)
			{
				if (enemyCounter == 150)
				{
					EnemyAttack();
					enemyCounter = 0;
					if (!playerDead) heroineCounter = 1;
					if (!mageDead) mageCounter = 1;
					if (!knightDead) knightCounter = 1;
					btnHeroine->state = GuiControlState::NORMAL;
					btnMage->state = GuiControlState::NORMAL;
					btnKnight->state = GuiControlState::NORMAL;
				}
			}

		}
		else
		{

			if (posX == 0&&!changed)
			{
				if (heroineCounter == 0 && mageCounter == 0)
				{
					posY = 2;
				}
				else if(heroineCounter == 0 && knightCounter == 0)
				{
					posY = 1;
				}
				else if (mageCounter == 0 && knightCounter == 0)
				{
					posY = 0;
				}
				else if (heroineCounter == 0)
				{
					posY = 1;
				}
				else
				{
					posY = 0;
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.a) { Select(); }

			if (posX == 0)
			{
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || pad.up)
				{
					changed = true;
					if (heroineCounter == 1 && mageCounter == 1 && knightCounter == 1)
					{
						posY--;
						if (posY < 0) posY = 2;
					}
					else if (heroineCounter == 1 && mageCounter == 1)
					{
						posY--;
						if (posY == 2) posY = 1;
						if (posY < 0) posY = 1;

					}
					else if (heroineCounter == 1 && knightCounter == 1)
					{
						posY--;
						if (posY == 1) posY = 0;
						if (posY < 0) posY = 2;

					}
					else if (mageCounter == 1 && knightCounter == 1)
					{
						posY--;
						if (posY < 1) posY = 2;

					}
					else if (heroineCounter == 1)
					{
						posY = 0;
					}
					else if (mageCounter == 1)
					{
						posY = 1;
					}
					else 
					{
						posY = 2;
					}

				}
			
				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || pad.down)
				{
					changed = true;
					if (heroineCounter == 1 && mageCounter == 1 && knightCounter == 1)
					{
						posY++;
						if (posY > 2) posY = 0;
					}
					else if (heroineCounter == 1 && mageCounter == 1)
					{
						posY++;
						if (posY > 1) posY = 0;
					}
					else if (heroineCounter == 1 && knightCounter == 1)
					{
						posY++;
						if (posY == 1) posY = 2;
						else if (posY > 2) posY = 0;
					}
					else if (mageCounter == 1 && knightCounter == 1)
					{
						posY++;
						if (posY == 0) posY = 1;
						else if (posY > 2) posY = 1;
					}
					else if (heroineCounter == 1)
					{
						posY = 0;
					}
					else if (mageCounter == 1)
					{
						posY = 1;
					}
					else
					{
						posY = 2;
					}
				}
			}
			else if (posX == 1)
			{
				changed = false;
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || pad.up)
				{
					posY--;
					if (posY < 0) posY = 2;
				}


				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || pad.down)
				{
					posY++;
					if (posY > 2) posY = 0;
				}
			}
			else if (posX == 2)
			{
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || pad.up)
				{

					if (enemiesAlive == 3 || enemiesAlive == 2)posY--;

					if (enemiesAlive == 3)
					{
						if (posY < 0) posY = 2;
					}
					else if (enemiesAlive == 2)
					{
						if (posY < 0) posY = 2;

						if (enemy1Dead && posY == 1) posY = 0;
						if (enemy2Dead && posY == 0) posY = 2;
						if (enemy3Dead && posY == 2) posY = 1;
					}
				}

				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || pad.down)
				{
					if (enemiesAlive == 3 || enemiesAlive == 2) posY++;
					if (enemiesAlive == 3)
					{
						if (posY > 2) posY = 0;
					}
					else if (enemiesAlive == 2)
					{
						if (posY > 2) posY = 0;
						if (enemy1Dead && posY == 1) posY = 0;
						if (enemy2Dead && posY == 0) posY = 1;
						if (enemy3Dead && posY == 2) posY = 2;
					}
				}
			}

		}


	}
	else //Heroine and Mage vs 3
	{
		if (enemiesAlive == 0 || (playerDead && mageDead))
		{
			app->fadeToBlack->FadeToBlk(this, app->scene, 30);
			app->fadeToBlack->animId = 1;
			battleOn = false;
			battleEnd = true;
		}
		btnHeroine->Update(dt);
		btnMage->Update(dt);
		if (heroine || mage)
		{
			btnAttack->Update(dt);
			btnMagic->Update(dt);
			btnDefense->Update(dt);
		}
		if (attack || magic)
		{
			btnEnemy1->Update(dt);
			btnEnemy2->Update(dt);
			btnEnemy3->Update(dt);
		}

		//dead
		if (player->playerHealth <= 0)
		{
			player->playerHealth = 0;
			heroineCounter = 0;
			playerDead = true;
			playerRevive = true;
		}
		if (npc5->mageHealth <= 0)
		{
			npc5->mageHealth = 0;
			mageCounter = 0;
			mageDead = true;
			mageRevive = true;
		}



		if (heroineCounter == 0 && mageCounter == 0)
		{

			enemyCounter++;
			if (enemiesAlive == 3)
			{
				if (enemyCounter == 150)
				{
					EnemyAttack();
				}
				if (enemyCounter == 250)
				{
					EnemyAttack();
				}
				if (enemyCounter == 350)
				{
					EnemyAttack();
					enemyCounter = 0;
					if (!playerDead) heroineCounter = 1;
					if (!mageDead) mageCounter = 1;
					btnHeroine->state = GuiControlState::NORMAL;
					btnMage->state = GuiControlState::NORMAL;
				}
			}
			else if (enemiesAlive == 2)
			{
				if (enemyCounter == 150)
				{
					EnemyAttack();
				}
				if (enemyCounter == 250)
				{
					EnemyAttack();
					enemyCounter = 0;
					if (!playerDead) heroineCounter = 1;
					if (!mageDead) mageCounter = 1;
					btnHeroine->state = GuiControlState::NORMAL;
					btnMage->state = GuiControlState::NORMAL;
				}
			}
			else if (enemiesAlive == 1)
			{
				if (enemyCounter == 150)
				{
					EnemyAttack();
					enemyCounter = 0;
					if (!playerDead) heroineCounter = 1;
					if (!mageDead) mageCounter = 1;
					btnHeroine->state = GuiControlState::NORMAL;
					btnMage->state = GuiControlState::NORMAL;
				}
			}

		}
		else
		{
			if (posX == 0 && !changed)
			{
				if (heroineCounter == 0)
				{
					posY = 1;
				}
				else
				{
					posY = 0;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) { Select(); }

			if (posX == 0)
			{
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
				{
					changed = true;
					if (heroineCounter == 1 && mageCounter==1)
					{
						posY--;
						if (posY < 0) posY = 1;
					}
					else if (mageCounter == 1)
					{
						posY = 1;
					}
				}


				if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
				{
					changed = true;

					if (heroineCounter == 1 && mageCounter==1)
					{
						posY++;
						if (posY > 1) posY = 0;
					}
					else if (mageCounter == 1)
					{
						posY = 1;
					}
				}
			}
			else if (posX == 1)
			{
				changed = false;

				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
				{
					posY--;
					if (posY < 0) posY = 2;
				}


				if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
				{
					posY++;
					if (posY > 2) posY = 0;
				}
			}
			else if (posX == 2)
			{
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
				{
					
					if (enemiesAlive == 3 || enemiesAlive == 2)posY--;
					
					if (enemiesAlive == 3)
					{
						if (posY < 0) posY = 2;
					}
					else if (enemiesAlive == 2)
					{
						if (posY < 0) posY = 2;

						if (enemy1Dead && posY == 1) posY = 0;
						if (enemy2Dead && posY == 0) posY = 2;
						if (enemy3Dead && posY == 2) posY = 1;
					}
				}

				if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
				{
					if (enemiesAlive == 3 || enemiesAlive == 2) posY++;	
					if (enemiesAlive == 3)
					{
						if (posY > 2) posY = 0;
					}
					else if (enemiesAlive == 2)
					{
						if (posY > 2) posY = 0;
						if (enemy1Dead && posY == 1) posY = 0;
						if (enemy2Dead && posY == 0) posY = 1;
						if (enemy3Dead && posY == 2) posY = 2;
					}
				}
			}
		}
	}
	return true;
}

// Update: draw background
bool SceneBattle::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	if (app->scene->knightTkn)
	{
		app->render->DrawTexture(battletext, 0, 0, fullscreenRect, 3);
		app->render->DrawRectangle({ 0, 480, 1280, 240 }, 0, 0, 0, 220, true, false);
		app->render->DrawRectangle({ 10, 490, 330, 220 }, 100, 100, 200, 220, true, false);
		app->render->DrawRectangle({ 350, 490, 370, 220 }, 100, 100, 200, 220, true, false);
		app->render->DrawRectangle({ 730, 490, 540, 220 }, 100, 100, 200, 220, true, false);

		app->font->DrawText(35, 167, goldFont, "NAME");
		// Print Characters HP
		app->font->DrawText(280, 167, goldFont, "HP");

		// Heroine
		sprintf_s(heroineHpText, 10, "%d/150", player->playerHealth);
		app->font->DrawText(280, 182, whiteFont, heroineHpText);

		// mage
		sprintf_s(mageHpText, 10, "%d/125", npc5->mageHealth);
		app->font->DrawText(280, 200, whiteFont, mageHpText);

		//knight
		sprintf_s(knightHpText, 10, "%d/200", npc7->knightHealth);
		app->font->DrawText(280, 218, whiteFont, knightHpText);

		// Print Enemies HP
		// Enemy Central
		if (!enemy1Dead)
		{
			sprintf_s(enemy1HpText, 10, "%d/100", enemy1->enemy1Health);
			app->font->DrawText(320, 100, whiteFont, enemy1HpText);
		}

		// Enemy Up
		if (!enemy2Dead)
		{
			sprintf_s(enemy2HpText, 10, "%d/150", enemy2->enemy2Health);
			app->font->DrawText(335, 75, whiteFont, enemy2HpText);
		}

		// Enemy Down
		if (!enemy3Dead)
		{
			sprintf_s(enemy3HpText, 10, "%d/50", enemy3->enemy3Health);
			app->font->DrawText(335, 120, whiteFont, enemy3HpText);
		}



		btnHeroine->Draw();
		btnMage->Draw();
		btnKnight->Draw();
		if (heroine || mage || knight)
		{
			btnAttack->Draw();
			btnMagic->Draw();
			btnDefense->Draw();
		}
		if (attack || magic)
		{
			btnEnemy1->Draw();
			btnEnemy2->Draw();
			btnEnemy3->Draw();
		}


		if (posX == 0 && posY == 0)
		{
			posScaleX = 5;
			posScaleY = 180;

		}
		if (posX == 0 && posY == 1)
		{
			posScaleX = 5;
			posScaleY = 198;
		}
		if (posX == 0 && posY == 2)
		{
			posScaleX = 5;
			posScaleY = 216;
		}

		if (posX == 1 && posY == 0)
		{
			posScaleX = 120;
			posScaleY = 173;
		}
		if (posX == 1 && posY == 1)
		{
			posScaleX = 120;
			posScaleY = 193;
		}
		if (posX == 1 && posY == 2)
		{
			posScaleX = 120;
			posScaleY = 213;
		}

		if (posX == 2 && posY == 0)
		{
			posScaleX = 295;
			posScaleY = 78;
		}
		if (posX == 2 && posY == 1)
		{
			posScaleX = 280;
			posScaleY = 98;
		}
		if (posX == 2 && posY == 2)
		{
			posScaleX = 295;
			posScaleY = 118;
		}

	}
	else //Heroine and Mage vs 3
	{
		app->render->DrawTexture(battletext, 0, 0, fullscreenRect, 3);
		app->render->DrawRectangle({ 0, 510, 1280, 210 }, 0, 0, 0, 220, true, false);
		app->render->DrawRectangle({ 10, 520, 330, 190 }, 100, 100, 200, 220, true, false);
		app->render->DrawRectangle({ 350, 520, 370, 190 }, 100, 100, 200, 220, true, false);
		app->render->DrawRectangle({ 730, 520, 540, 190 }, 100, 100, 200, 220, true, false);

		app->font->DrawText(35, 180, goldFont, "NAME");
		// Print Characters HP
		app->font->DrawText(280, 180, goldFont, "HP");
		// Heroine

		sprintf_s(heroineHpText, 10, "%d/150", player->playerHealth);
		app->font->DrawText(280, 200, whiteFont, heroineHpText);

		// Heroine

		sprintf_s(mageHpText, 10, "%d/125", npc5->mageHealth);
		app->font->DrawText(280, 220, whiteFont, mageHpText);

		// Print Enemies HP
		// Enemy Central
		if (!enemy1Dead)
		{
			sprintf_s(enemy1HpText, 10, "%d/100", enemy1->enemy1Health);
			app->font->DrawText(320, 100, whiteFont, enemy1HpText);
		}

		// Enemy Up
		if (!enemy2Dead)
		{
			sprintf_s(enemy2HpText, 10, "%d/150", enemy2->enemy2Health);
			app->font->DrawText(335, 75, whiteFont, enemy2HpText);
		}

		// Enemy Down
		if (!enemy3Dead)
		{
			sprintf_s(enemy3HpText, 10, "%d/50", enemy3->enemy3Health);
			app->font->DrawText(335, 120, whiteFont, enemy3HpText);
		}



		btnHeroine->Draw();
		btnMage->Draw();
		if (heroine || mage)
		{
			btnAttack->Draw();
			btnMagic->Draw();
			btnDefense->Draw();
		}
		if (attack || magic)
		{
			btnEnemy1->Draw();
			btnEnemy2->Draw();
			btnEnemy3->Draw();
		}

		if (posX == 0 && posY == 0)
		{
			posScaleX = 5;
			posScaleY = 198;

		}
		if (posX == 0 && posY == 1)
		{
			posScaleX = 5;
			posScaleY = 218;
		}

		if (posX == 1 && posY == 0)
		{
			posScaleX = 120;
			posScaleY = 178;
		}
		if (posX == 1 && posY == 1)
		{
			posScaleX = 120;
			posScaleY = 198;
		}
		if (posX == 1 && posY == 2)
		{
			posScaleX = 120;
			posScaleY = 218;
		}

		if (posX == 2 && posY == 0)
		{
			posScaleX = 295;
			posScaleY = 78;
		}
		if (posX == 2 && posY == 1)
		{
			posScaleX = 280;
			posScaleY = 98;
		}
		if (posX == 2 && posY == 2)
		{
			posScaleX = 295;
			posScaleY = 118;
		}


	}

	app->render->DrawTexture(selectorTex, posScaleX, posScaleY, NULL);

	return ret;
}


void SceneBattle::Select()
{

	if (app->scene->knightTkn) 
	{

		if (posX == 0)
		{
			if (posY == 0 && heroineCounter == 1 && !playerDead)
			{
				heroine = true;
				mage = false;
				knight = false;
				btnHeroine->state = GuiControlState::DISABLED;
				btnMage->state = GuiControlState::NORMAL;
				btnKnight->state = GuiControlState::NORMAL;
			}
			if (posY == 1 && mageCounter == 1 && !mageDead)
			{
				mage = true;
				heroine = false;
				knight = false;
				btnMage->state = GuiControlState::DISABLED;
				btnHeroine->state = GuiControlState::NORMAL;
				btnKnight->state = GuiControlState::NORMAL;
			}
			else if (posY == 2 && (knightCounter == 1) && !knightDead)
			{
				mage = false;
				heroine = false;
				knight = true;
				btnMage->state = GuiControlState::NORMAL;
				btnHeroine->state = GuiControlState::NORMAL;
				btnKnight->state = GuiControlState::DISABLED;
			}
			posX = 1;
			posY = 0;
		}
		else if (posX == 1)
		{
			if (posY == 0)
			{
				attack = true;
				magic = false;
				btnAttack->state = GuiControlState::DISABLED;
				btnMagic->state = GuiControlState::NORMAL;
				btnDefense->state = GuiControlState::NORMAL;
			}
			if (posY == 1)
			{
				magic = true;
				attack = false;
				btnAttack->state = GuiControlState::NORMAL;
				btnMagic->state = GuiControlState::DISABLED;
				btnDefense->state = GuiControlState::NORMAL;
			}
			if (posY == 2)
			{
				if (heroine)
				{
					hDefense = true;
					heroineCounter = 0;
				}
				else if (mage)
				{
					mDefense = true;
					mageCounter = 0;
				}
				else if (knight)
				{
					kDefense = true;
					knightCounter = 0;
				}

				heroine = false;
				mage = false;
				knight = false;
				attack = false;
				magic = false;


			}
			if (posY != 2)
			{
				if (enemy2Dead)
				{
					if (enemy1Dead)
					{
						posY = 2;
					}
					else
					{
						posY = 1;
					}
				}
				else
				{
					posY = 0;
				}
				posX = 2;
			}
			else {

				if (heroineCounter == 0)
				{
					if (mageCounter == 0)posY = 2;
					else posY = 1;
					
				}
				else
				{
					posY = 0;
				}
				posX = 0;
			}


		}
		else if (posX == 2)
		{

			if (posY == 1 && !enemy1Dead)
			{
				// Enemy Central
				if (heroine)
				{
					if (attack)
					{
						enemy1->enemy1Health -= player->playerDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						heroineCounter = 0;
					}
					else if (magic)
					{
						enemy1->enemy1Health -= player->playerMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						heroineCounter = 0;
					}
					heroine = false;
				}
				else if (mage)
				{
					if (attack)
					{
						enemy1->enemy1Health -= npc5->mageDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						mageCounter = 0;
					}
					else if (magic)
					{
						enemy1->enemy1Health -= npc5->mageMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						mageCounter = 0;
					}
					mage = false;

				}
				else if (knight)
				{
					if (attack)
					{
						enemy1->enemy1Health -= npc7->knightDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						knightCounter = 0;
					}
					else if (magic)
					{
						enemy1->enemy1Health -= npc7->knightMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						knightCounter = 0;
					}
					knight = false;
				}

				// Resetting button states
				btnAttack->state = GuiControlState::NORMAL;
				btnMagic->state = GuiControlState::NORMAL;
				posX = 0;
				if (heroineCounter == 0)
				{
					if (mageCounter == 0)posY = 2;
					else posY = 1;
				}
				else
				{
					posY = 0;
				}

				if (enemy1->enemy1Health <= 0)
				{
					enemy1->enemy1Health = 0;
					enemy1Dead = true;
					enemiesAlive--;
				}
			}
			else if (posY == 0 && !enemy2Dead)
			{
				// Enemy Up
				if (heroine)
				{
					if (attack)
					{
						enemy2->enemy2Health -= player->playerDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						heroineCounter = 0;
					}
					else if (magic)
					{
						enemy2->enemy2Health -= player->playerMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						heroineCounter = 0;
					}
					heroine = false;

				}
				else if (mage)
				{
					if (attack)
					{
						enemy2->enemy2Health -= npc5->mageDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						mageCounter = 0;
					}
					else if (magic)
					{
						enemy2->enemy2Health -= npc5->mageMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						mageCounter = 0;
					}
					mage = false;
				}
				else if (knight)
				{
					if (attack)
					{
						enemy2->enemy2Health -= npc7->knightDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						knightCounter = 0;
					}
					else if (magic)
					{
						enemy2->enemy2Health -= npc7->knightMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						knightCounter = 0;
					}
					knight = false;
				}

				// Resetting button states
				btnAttack->state = GuiControlState::NORMAL;
				btnMagic->state = GuiControlState::NORMAL;
				posX = 0;
				if (heroineCounter == 0)
				{
					if (mageCounter == 0)posY = 2;
					else posY = 1;
				}
				else
				{
					posY = 0;
				}
				if (enemy2->enemy2Health <= 0)
				{
					enemy2->enemy2Health = 0;
					enemy2Dead = true;
					enemiesAlive--;
				}
			}
			else if (posY == 2 && !enemy3Dead)
			{
				// Enemy Down
				if (heroine)
				{
					if (attack)
					{
						enemy3->enemy3Health -= player->playerDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						heroineCounter = 0;
					}
					else if (magic)
					{
						enemy3->enemy3Health -= player->playerMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						heroineCounter = 0;
					}
					heroine = false;
				}
				else if (mage)
				{
					if (attack)
					{
						enemy3->enemy3Health -= npc5->mageDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						mageCounter = 0;
					}
					else if (magic)
					{
						enemy3->enemy3Health -= npc5->mageMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						mageCounter = 0;
					}
					mage = false;
				}
				else if (knight)
				{
					if (attack)
					{
						enemy3->enemy3Health -= npc7->knightDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						knightCounter = 0;
					}
					else if (magic)
					{
						enemy3->enemy3Health -= npc7->knightMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						knightCounter = 0;
					}
					knight = false;
				}

				// Resetting button states
				btnAttack->state = GuiControlState::NORMAL;
				btnMagic->state = GuiControlState::NORMAL;
				posX = 0;
				if (heroineCounter == 0)
				{
					if (mageCounter == 0)posY = 2;
					else posY = 1;
				}
				else
				{
					posY = 0;
				}
				if (enemy3->enemy3Health <= 0)
				{
					enemy3->enemy3Health = 0;
					enemy3Dead = true;
					enemiesAlive--;
				}
			}
		}


	} //Heroine and mage vs 3 enemies
	else
	{
		if (posX == 0)
		{
			if (posY == 0 && heroineCounter == 1 && !playerDead)
			{
				heroine = true;
				mage = false;
				knight = false;
				btnHeroine->state = GuiControlState::DISABLED;
				btnMage->state = GuiControlState::NORMAL;
			}
			if (posY == 1 && mageCounter == 1 && !mageDead)
			{
				mage = true;
				heroine = false;
				knight = false;
				btnMage->state = GuiControlState::DISABLED;
				btnHeroine->state = GuiControlState::NORMAL;
			}
			posX = 1;
			posY = 0;
		}
		else if (posX == 1)
		{
			if (posY == 0)
			{
				attack = true;
				magic = false;
				btnAttack->state = GuiControlState::DISABLED;
				btnMagic->state = GuiControlState::NORMAL;
				btnDefense->state = GuiControlState::NORMAL;
			}
			if (posY == 1)
			{
				magic = true;
				attack = false;
				btnAttack->state = GuiControlState::NORMAL;
				btnMagic->state = GuiControlState::DISABLED;
				btnDefense->state = GuiControlState::NORMAL;
			}
			if (posY == 2)
			{
				if (heroine)
				{
					hDefense = true;
					heroineCounter = 0;
				}
				else if (mage)
				{
					mDefense = true;
					mageCounter = 0;
				}


				heroine = false;
				mage = false;
				knight = false;
				attack = false;
				magic = false;


			}
			if (posY != 2)
			{
				if (enemy2Dead)
				{
					if (enemy1Dead)
					{
						posY = 2;
					}
					else
					{
						posY = 1;
					}
				}
				else
				{
					posY = 0;
				}
				posX = 2;
			}
			else {

				if (heroineCounter == 0)
				{
					posY = 1;
				}
				else
				{
					posY = 0;
				}
				posX = 0;
			}


		}
		else if (posX == 2)
		{

			if (posY == 1 && !enemy1Dead)
			{
				// Enemy Central
				if (heroine)
				{
					if (attack)
					{
						enemy1->enemy1Health -= player->playerDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						heroineCounter = 0;
					}
					else if (magic)
					{
						enemy1->enemy1Health -= player->playerMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						heroineCounter = 0;
					}
					heroine = false;
				}
				else if (mage)
				{
					if (attack)
					{
						enemy1->enemy1Health -= npc5->mageDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						mageCounter = 0;
					}
					else if (magic)
					{
						enemy1->enemy1Health -= npc5->mageMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						mageCounter = 0;
					}
					mage = false;

				}

				// Resetting button states
				btnAttack->state = GuiControlState::NORMAL;
				btnMagic->state = GuiControlState::NORMAL;
				posX = 0;
				if (heroineCounter == 0)
				{
					posY = 1;
				}
				else
				{
					posY = 0;
				}

				if (enemy1->enemy1Health <= 0)
				{
					enemy1->enemy1Health = 0;
					enemy1Dead = true;
					enemiesAlive--;
				}
			}
			else if (posY == 0 && !enemy2Dead)
			{
				// Enemy Up
				if (heroine)
				{
					if (attack)
					{
						enemy2->enemy2Health -= player->playerDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						heroineCounter = 0;
					}
					else if (magic)
					{
						enemy2->enemy2Health -= player->playerMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						heroineCounter = 0;
					}
					heroine = false;

				}
				else if (mage)
				{
					if (attack)
					{
						enemy2->enemy2Health -= npc5->mageDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						mageCounter = 0;
					}
					else if (magic)
					{
						enemy2->enemy2Health -= npc5->mageMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						mageCounter = 0;
					}
					mage = false;
				}

				// Resetting button states
				btnAttack->state = GuiControlState::NORMAL;
				btnMagic->state = GuiControlState::NORMAL;
				posX = 0;
				if (heroineCounter == 0)
				{
					posY = 1;
				}
				else
				{
					posY = 0;
				}
				if (enemy2->enemy2Health <= 0)
				{
					enemy2->enemy2Health = 0;
					enemy2Dead = true;
					enemiesAlive--;
				}
			}
			else if (posY == 2 && !enemy3Dead)
			{
				// Enemy Down
				if (heroine)
				{
					if (attack)
					{
						enemy3->enemy3Health -= player->playerDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						heroineCounter = 0;
					}
					else if (magic)
					{
						enemy3->enemy3Health -= player->playerMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						heroineCounter = 0;
					}
					heroine = false;
				}
				else if (mage)
				{
					if (attack)
					{
						enemy3->enemy3Health -= npc5->mageDmg;
						attack = false;
						app->audio->PlayFx(hitEnemyFx, 0);
						mageCounter = 0;
					}
					else if (magic)
					{
						enemy3->enemy3Health -= npc5->mageMagicDmg;
						magic = false;
						app->audio->PlayFx(magicEnemyFx, 0);
						mageCounter = 0;
					}
					mage = false;
				}
				// Resetting button states
				btnAttack->state = GuiControlState::NORMAL;
				btnMagic->state = GuiControlState::NORMAL;
				posX = 0;
				if (heroineCounter == 0)
				{
					posY = 1;
				}
				else
				{
					posY = 0;
				}
				if (enemy3->enemy3Health <= 0)
				{
					enemy3->enemy3Health = 0;
					enemy3Dead = true;
					enemiesAlive--;
				}
			}
		}
	}
	
}

//
//bool SceneBattle::OnGuiMouseClickEvent(GuiControl* control)
//{
//
//
//	switch (control->type)
//	{
//	case GuiControlType::BUTTON:
//	{
//		if (app->scene->knightTkn)
//		{
//			if ((control->id == 1) && (heroineCounter == 1) && !playerDead)
//			{
//				heroine = true;
//				mage = false;
//				knight = false;
//				btnHeroine->state = GuiControlState::DISABLED;
//				btnMage->state = GuiControlState::NORMAL;
//				btnKnight->state = GuiControlState::NORMAL;
//			}
//			else if ((control->id == 2) && (mageCounter == 1) && !mageDead)
//			{
//				mage = true;
//				heroine = false;
//				knight = false;
//				btnMage->state = GuiControlState::DISABLED;
//				btnHeroine->state = GuiControlState::NORMAL;
//				btnKnight->state = GuiControlState::NORMAL;
//			}
//			else if ((control->id == 9) && (knightCounter == 1) && !knightDead)
//			{
//				mage = false;
//				heroine = false;
//				knight = true;
//				btnMage->state = GuiControlState::NORMAL;
//				btnHeroine->state = GuiControlState::NORMAL;
//				btnKnight->state = GuiControlState::DISABLED;
//			}
//			else if (control->id == 3)
//			{
//				attack = true;
//				magic = false;
//				btnAttack->state = GuiControlState::DISABLED;
//				btnMagic->state = GuiControlState::NORMAL;
//				btnDefense->state = GuiControlState::NORMAL;
//			}
//			else if (control->id == 4)
//			{
//				magic = true;
//				attack = false;
//				btnAttack->state = GuiControlState::NORMAL;
//				btnMagic->state = GuiControlState::DISABLED;
//				btnDefense->state = GuiControlState::NORMAL;
//			}
//			else if (control->id == 5)
//			{
//				if (heroine)
//				{
//					hDefense = true;
//					heroineCounter = 0;
//				}
//				else if (mage)
//				{
//					mDefense = true;
//					mageCounter = 0;
//				}
//				else if (knight)
//				{
//					kDefense = true;
//					knightCounter = 0;
//				}
//
//				heroine = false;
//				mage = false;
//				knight = false;
//				attack = false;
//				magic = false;
//			}
//			else if (control->id == 6 && !enemy1Dead)
//			{
//				// Enemy Central
//				if (heroine)
//				{
//					if (attack)
//					{
//						enemy1->enemy1Health -= player->playerDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy1->enemy1Health -= player->playerMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					heroine = false;
//				}
//				else if (mage)
//				{
//					if (attack)
//					{
//						enemy1->enemy1Health -= npc5->mageDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						mageCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy1->enemy1Health -= npc5->mageMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						mageCounter = 0;
//					}
//					mage = false;
//				}
//				else if (knight)
//				{
//					if (attack)
//					{
//						enemy1->enemy1Health -= npc7->knightDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						knightCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy1->enemy1Health -= npc7->knightMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						knightCounter = 0;
//					}
//					knight = false;
//				}
//
//				// Resetting button states
//				btnAttack->state = GuiControlState::NORMAL;
//				btnMagic->state = GuiControlState::NORMAL;
//
//				if (enemy1->enemy1Health <= 0)
//				{
//					enemy1->enemy1Health = 0;
//					enemy1Dead = true;
//					enemiesAlive--;
//				}
//			}
//			else if (control->id == 7 && !enemy2Dead)
//			{
//				// Enemy Up
//				if (heroine)
//				{
//					if (attack)
//					{
//						enemy2->enemy2Health -= player->playerDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy2->enemy2Health -= player->playerMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					heroine = false;
//				}
//				else if (mage)
//				{
//					if (attack)
//					{
//						enemy2->enemy2Health -= npc5->mageDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						mageCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy2->enemy2Health -= npc5->mageMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						mageCounter = 0;
//					}
//					mage = false;
//				}
//				else if (knight)
//				{
//					if (attack)
//					{
//						enemy2->enemy2Health -= npc7->knightDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						knightCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy2->enemy2Health -= npc7->knightMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						knightCounter = 0;
//					}
//					knight = false;
//				}
//
//				// Resetting button states
//				btnAttack->state = GuiControlState::NORMAL;
//				btnMagic->state = GuiControlState::NORMAL;
//
//				if (enemy2->enemy2Health <= 0)
//				{
//					enemy2->enemy2Health = 0;
//					enemy2Dead = true;
//					enemiesAlive--;
//				}
//			}
//			else if (control->id == 8 && !enemy3Dead)
//			{
//				// Enemy Down
//				if (heroine)
//				{
//					if (attack)
//					{
//						enemy3->enemy3Health -= player->playerDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy3->enemy3Health -= player->playerMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					heroine = false;
//				}
//				else if (mage)
//				{
//					if (attack)
//					{
//						enemy3->enemy3Health -= npc5->mageDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						mageCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy3->enemy3Health -= npc5->mageMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						mageCounter = 0;
//					}
//					mage = false;
//				}
//				else if (knight)
//				{
//					if (attack)
//					{
//						enemy3->enemy3Health -= npc7->knightDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						knightCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy3->enemy3Health -= npc7->knightMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						knightCounter = 0;
//					}
//					knight = false;
//				}
//
//				// Resetting button states
//				btnAttack->state = GuiControlState::NORMAL;
//				btnMagic->state = GuiControlState::NORMAL;
//
//				if (enemy3->enemy3Health <= 0)
//				{
//					enemy3->enemy3Health = 0;
//					enemy3Dead = true;
//					enemiesAlive--;
//				}
//			}
//		}
//		else // Heroine and Mage vs 3
//		{
//			if ((control->id == 1) && (heroineCounter == 1) && !playerDead)
//			{
//				heroine = true;
//				mage = false;
//				btnHeroine->state = GuiControlState::DISABLED;
//				btnMage->state = GuiControlState::NORMAL;
//			}
//			else if ((control->id == 2) && (mageCounter == 1) && !mageDead)
//			{
//				mage = true;
//				heroine = false;
//				btnMage->state = GuiControlState::DISABLED;
//				btnHeroine->state = GuiControlState::NORMAL;
//			}
//
//			else if (control->id == 3)
//			{
//				attack = true;
//				magic = false;
//				btnAttack->state = GuiControlState::DISABLED;
//				btnMagic->state = GuiControlState::NORMAL;
//				btnDefense->state = GuiControlState::NORMAL;
//			}
//			else if (control->id == 4)
//			{
//				magic = true;
//				attack = false;
//				btnAttack->state = GuiControlState::NORMAL;
//				btnMagic->state = GuiControlState::DISABLED;
//				btnDefense->state = GuiControlState::NORMAL;
//			}
//			else if (control->id == 5)
//			{
//				if (heroine)
//				{
//					hDefense = true;
//					heroineCounter = 0;
//				}
//				else if (mage)
//				{
//					mDefense = true;
//					mageCounter = 0;
//				}
//
//				heroine = false;
//				mage = false;
//				attack = false;
//				magic = false;
//			}
//			else if (control->id == 6 && !enemy1Dead)
//			{
//				// Enemy Central
//				if (heroine)
//				{
//					if (attack)
//					{
//						enemy1->enemy1Health -= player->playerDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy1->enemy1Health -= player->playerMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					heroine = false;
//				}
//				else if (mage)
//				{
//					if (attack)
//					{
//						enemy1->enemy1Health -= npc5->mageDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						mageCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy1->enemy1Health -= npc5->mageMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						mageCounter = 0;
//					}
//					mage = false;
//				}
//
//				// Resetting button states
//				btnAttack->state = GuiControlState::NORMAL;
//				btnMagic->state = GuiControlState::NORMAL;
//
//				if (enemy1->enemy1Health <= 0)
//				{
//					enemy1->enemy1Health = 0;
//					enemy1Dead = true;
//					enemiesAlive--;
//				}
//			}
//			else if (control->id == 7 && !enemy2Dead)
//			{
//				// Enemy Up
//				if (heroine)
//				{
//					if (attack)
//					{
//						enemy2->enemy2Health -= player->playerDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy2->enemy2Health -= player->playerMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					heroine = false;
//				}
//				else if (mage)
//				{
//					if (attack)
//					{
//						enemy2->enemy2Health -= npc5->mageDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						mageCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy2->enemy2Health -= npc5->mageMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						mageCounter = 0;
//					}
//					mage = false;
//				}
//
//				// Resetting button states
//				btnAttack->state = GuiControlState::NORMAL;
//				btnMagic->state = GuiControlState::NORMAL;
//
//				if (enemy2->enemy2Health <= 0)
//				{
//					enemy2->enemy2Health = 0;
//					enemy2Dead = true;
//					enemiesAlive--;
//				}
//			}
//			else if (control->id == 8 && !enemy3Dead)
//			{
//				// Enemy Down
//				if (heroine)
//				{
//					if (attack)
//					{
//						enemy3->enemy3Health -= player->playerDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy3->enemy3Health -= player->playerMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						heroineCounter = 0;
//					}
//					heroine = false;
//				}
//				else if (mage)
//				{
//					if (attack)
//					{
//						enemy3->enemy3Health -= npc5->mageDmg;
//						attack = false;
//						app->audio->PlayFx(hitEnemyFx, 0);
//						mageCounter = 0;
//					}
//					else if (magic)
//					{
//						enemy3->enemy3Health -= npc5->mageMagicDmg;
//						magic = false;
//						app->audio->PlayFx(magicEnemyFx, 0);
//						mageCounter = 0;
//					}
//					mage = false;
//				}
//				// Resetting button states
//				btnAttack->state = GuiControlState::NORMAL;
//				btnMagic->state = GuiControlState::NORMAL;
//
//				if (enemy3->enemy3Health <= 0)
//				{
//					enemy3->enemy3Health = 0;
//					enemy3Dead = true;
//					enemiesAlive--;
//				}
//			}
//		}
//	}
//	default: break;
//	}
//
//	return true;
//}
//

bool SceneBattle::CleanUp()
{
	app->tex->UnLoad(battletext);
	app->tex->UnLoad(selectorTex);
	app->font->UnLoad(whiteFont);
	app->font->UnLoad(goldFont);
	app->entityManager->DestroyEntity(player);
	app->entityManager->DestroyEntity(enemy1);
	app->entityManager->DestroyEntity(enemy2);
	app->entityManager->DestroyEntity(enemy3);
	app->entityManager->DestroyEntity(npc5);
	app->entityManager->DestroyEntity(npc7);

	app->entityManager->Disable();
	return true;
}

void SceneBattle::EnemyAttack()
{
	if (app->scene->knightTkn)
	{
		srand(time(NULL));
		int enemyPool = rand() % 3 + 1;

		int characterPool = rand() % 3 + 1;

		while ((playerDead && characterPool == 1) || (mageDead && characterPool == 2) || (knightDead && characterPool == 3))
		{

			if (playerDead && characterPool == 1)
			{
				while (characterPool == 1) characterPool = rand() % 3 + 2;
			}
			else if (mageDead && characterPool == 2)
			{
				while (characterPool == 2) characterPool = rand() % 3 + 1;
			}
			else if (knightDead && characterPool == 3) characterPool = rand() % 2 + 1;
		}

		switch (enemyPool)
		{
		case 1:
		{
			// Enemy 1
			switch (characterPool)
			{
			case 1:
			{
				if (hDefense)
				{
					player->playerHealth -= (enemy1->enemy1Dmg) / 2;
				}
				else
				{
					player->playerHealth -= enemy1->enemy1Dmg;
				}
			} break;
			case 2:
			{
				if (mDefense)
				{
					npc5->mageHealth -= (enemy1->enemy1Dmg) / 2;
				}
				else
				{
					npc5->mageHealth -= enemy1->enemy1Dmg;
				}
			} break;
			case 3:
			{
				if (kDefense)
				{
					npc7->knightHealth -= (enemy1->enemy1Dmg) / 2;
				}
				else
				{
					npc7->knightHealth -= enemy1->enemy1Dmg;
				}
			} break;
			}
		} break;
		case 2:
		{
			// Enemy 2
			switch (characterPool)
			{
			case 1:
			{
				if (hDefense)
				{
					player->playerHealth -= (enemy2->enemy2Dmg) / 2;
				}
				else
				{
					player->playerHealth -= enemy2->enemy2Dmg;
				}
			} break;
			case 2:
			{
				if (mDefense)
				{
					npc5->mageHealth -= (enemy2->enemy2Dmg) / 2;
				}
				else
				{
					npc5->mageHealth -= enemy2->enemy2Dmg;
				}
			} break;
			case 3:
			{
				if (kDefense)
				{
					npc7->knightHealth -= (enemy2->enemy2Dmg) / 2;
				}
				else
				{
					npc7->knightHealth -= enemy2->enemy2Dmg;
				}
			} break;
			}
		} break;
		case 3:
		{
			// Enemy 3
			switch (characterPool)
			{
			case 1:
			{
				if (hDefense)
				{
					player->playerHealth -= (enemy3->enemy3Dmg) / 2;
				}
				else
				{
					player->playerHealth -= enemy3->enemy3Dmg;
				}
			} break;
			case 2:
			{
				if (mDefense)
				{
					npc5->mageHealth -= (enemy3->enemy3Dmg) / 2;
				}
				else
				{
					npc5->mageHealth -= enemy3->enemy3Dmg;
				}
			} break;
			case 3:
			{
				if (kDefense)
				{
					npc7->knightHealth -= (enemy3->enemy3Dmg) / 2;
				}
				else
				{
					npc7->knightHealth -= enemy3->enemy3Dmg;
				}
			} break;
			}
		} break;
		}
		app->audio->PlayFx(hitEnemyFx, 0);

	}
	else //Heroine and Mage vs 3 enemies
	{
	srand(time(NULL));
	int max = 3;
	int min = 2;
	int enemyPool = rand() % (max - min + 1) + min;

	int characterPool = (rand() % 2) + 1;

	if (playerDead && characterPool == 1)
	{
		characterPool = 2;
	}
	if (mageDead && characterPool == 2)
	{
		characterPool = 1;
	}

	switch (enemyPool)
	{
	case 1:
	{
		// Enemy 1
		switch (characterPool)
		{
		case 1:
		{
			if (hDefense)
			{
				player->playerHealth -= (enemy1->enemy1Dmg) / 2;
			}
			else
			{
				player->playerHealth -= enemy1->enemy1Dmg;
			}
		} break;
		case 2:
		{
			if (mDefense)
			{
				npc5->mageHealth -= (enemy1->enemy1Dmg) / 2;
			}
			else
			{
				npc5->mageHealth -= enemy1->enemy1Dmg;
			}
		} break;
		}
	} break;
	case 2:
	{
		// Enemy 2
		switch (characterPool)
		{
		case 1:
		{
			if (hDefense)
			{
				player->playerHealth -= (enemy2->enemy2Dmg) / 2;
			}
			else
			{
				player->playerHealth -= enemy2->enemy2Dmg;
			}
		} break;
		case 2:
		{
			if (mDefense)
			{
				npc5->mageHealth -= (enemy2->enemy2Dmg) / 2;
			}
			else
			{
				npc5->mageHealth -= enemy2->enemy2Dmg;
			}
		} break;
		}
	} break;
	case 3:
	{
		// Enemy 3
		switch (characterPool)
		{
		case 1:
		{
			if (hDefense)
			{
				player->playerHealth -= (enemy3->enemy3Dmg) / 2;
			}
			else
			{
				player->playerHealth -= enemy3->enemy3Dmg;
			}
		} break;
		case 2:
		{
			if (mDefense)
			{
				npc5->mageHealth -= (enemy3->enemy3Dmg) / 2;
			}
			else
			{
				npc5->mageHealth -= enemy3->enemy3Dmg;
			}
		} break;
		}
	} break;
	}
	app->audio->PlayFx(hitEnemyFx, 0);

	}
}