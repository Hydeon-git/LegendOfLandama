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
	return ret;
}

bool SceneBattle::Update(float dt)
{
	if (enemiesAlive == 0 || (playerDead && mageDead))
	{
		app->fadeToBlack->FadeToBlk(this, app->scene, 30);
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

	if (player->playerHealth <= 0)
	{
		player->playerHealth = 0;
		playerDead = true;
	}
	if (npc5->mageHealth <= 0)
	{
		npc5->mageHealth = 0;
		mageDead = true;
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
				heroineCounter = 1;
				mageCounter = 1;
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
				heroineCounter = 1;
				mageCounter = 1;
			}
		}
		else if (enemiesAlive == 1)
		{
			if (enemyCounter == 150)
			{
				EnemyAttack();
				enemyCounter = 0;
				heroineCounter = 1;
				mageCounter = 1;
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

	app->render->DrawTexture(battletext, 0, 0, fullscreenRect, 3);
	app->render->DrawRectangle({ 0, 510, 1280, 210 }, 0, 0, 0, 220, true, false);
	app->render->DrawRectangle({ 10, 520, 330, 190 }, 100, 100, 200, 220, true, false);
	app->render->DrawRectangle({ 350, 520, 370, 190 }, 100, 100, 200, 220, true, false);
	app->render->DrawRectangle({ 730, 520, 540, 190 }, 100, 100, 200, 220, true, false);

	app->font->DrawText(35, 180, goldFont, "NAME");
	// Print Characters HP
	app->font->DrawText(280, 180, goldFont, "HP");
	// Heroine

	sprintf_s(heroineHpText, 10, "%d/200", player->playerHealth);
	app->font->DrawText(280, 200, whiteFont, heroineHpText);

	// Heroine

	sprintf_s(mageHpText, 10, "%d/120", npc5->mageHealth);
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
	return ret;
}

bool SceneBattle::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if ((control->id == 1) && (heroineCounter == 1) && !playerDead)
		{
			heroine = true;
			mage = false;
			btnHeroine->state = GuiControlState::DISABLED;
			btnMage->state = GuiControlState::NORMAL;
		}
		else if ((control->id == 2) && (mageCounter == 1) && !mageDead)
		{
			mage = true;
			heroine = false;
			btnMage->state = GuiControlState::DISABLED;
			btnHeroine->state = GuiControlState::NORMAL;
		}

		else if (control->id == 3)
		{
			attack = true;
			magic = false;
			btnAttack->state = GuiControlState::DISABLED;
			btnMagic->state = GuiControlState::NORMAL;
			btnDefense->state = GuiControlState::NORMAL;
		}
		else if (control->id == 4)
		{
			magic = true;
			attack = false;
			btnAttack->state = GuiControlState::NORMAL;
			btnMagic->state = GuiControlState::DISABLED;
			btnDefense->state = GuiControlState::NORMAL;
		}
		else if (control->id == 5)
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
			attack = false;
			magic = false;
		}
		else if (control->id == 6 && !enemy1Dead)
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

			if (enemy1->enemy1Health <= 0)
			{
				enemy1->enemy1Health = 0;
				enemy1Dead = true;
				enemiesAlive--;
			}
		}
		else if (control->id == 7 && !enemy2Dead)
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

			if (enemy2->enemy2Health <= 0)
			{
				enemy2->enemy2Health = 0;
				enemy2Dead = true;
				enemiesAlive--;
			}
		}
		else if (control->id == 8 && !enemy3Dead)
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

			if (enemy3->enemy3Health <= 0)
			{
				enemy3->enemy3Health = 0;
				enemy3Dead = true;
				enemiesAlive--;
			}
		}
	}
	default: break;
	}

	return true;
}

bool SceneBattle::CleanUp()
{
	app->tex->UnLoad(battletext);
	app->font->UnLoad(whiteFont);
	app->font->UnLoad(goldFont);
	app->entityManager->DestroyEntity(player);
	app->entityManager->DestroyEntity(enemy1);
	app->entityManager->DestroyEntity(enemy2);
	app->entityManager->DestroyEntity(enemy3);
	app->entityManager->DestroyEntity(npc5);
	app->entityManager->DestroyEntity(npc7);
	return true;
}

void SceneBattle::EnemyAttack()
{

	int enemyPool = 1 + (rand() % 3);

	int characterPool = 1 + (rand() % 2);

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