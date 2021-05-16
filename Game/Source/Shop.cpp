#include "Shop.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Font.h"
#include "Player.h"
#include <SDL_mixer\include\SDL_mixer.h>
#include <SDL\include\SDL_keyboard.h>
#include "stdio.h"
#include "SteelSword.h"
#include "GoldSword.h"
#include "FireSword.h"
#include "WoodShield.h"
#include "SteelShield.h"
#include "GoldShield.h"
#include "FireShield.h"
#include "WoodSword.h"
#include "WoodBow.h"
#include "SteelBow.h"
#include "DialogSystem.h"
#include "HUD.h"

#include "GoldBow.h"
#include "DiamondBow.h"
#include "WoodWand.h"
#include "AmatistWand.h"
#include "RubyWand.h"
#include "DiamondWand.h"
#include "Item.h"
#include "App.h"
#include "Audio.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Font.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "FadeToBlack.h"
#include "Shop.h"
#include "SceneIntro.h"
#include "GuiButton.h"
#include <string>
#include <stdio.h>
#include "Quests.h"


#include <iostream>

#include "Defs.h"
#include "Log.h"

Shop::Shop() : Module()
{
	name.Create("Shop");
}

Shop::~Shop()
{

}

bool Shop::Awake(pugi::xml_node& node)
{
	return true;
}

bool Shop::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		
		LOG("Loading background assets");
		shopTex = app->tex->Load("Assets/Textures/shop.png");
		itemSold = app->tex->Load("Assets/Textures/shop_owned.png");
		selectorTex = app->tex->Load("Assets/Textures/pointer.png");
		buyFx = app->audio->LoadFx("Assets/Audio/Fx/buy_item.wav");
		selectorActive = false;
		shopX = 1;
		shopY = 1;


	}
	return ret;
}

bool Shop::Update(float dt)
{
	GamePad& pad = app->input->pads[0];

	if (!selectorActive) return true;
	
	itemPos = (shopX + (4 * (shopY - 1)))-1;

	if (open == true) 
	{
		app->scene->player->dialogeOn = false;
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.a) { select(); }
	

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || pad.up)
	{
		shopY--;
		if (shopY <= 0)shopY = 4;
	}


	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || pad.down)
	{
		shopY ++;
		if (shopY >=5)shopY = 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN || pad.right)
	{
		shopX --;
		if (shopX <= 0) {
			shopX = 4;
		}

	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN || pad.left)
	{
		shopX ++;
		if (shopX >= 5) {
			shopX = 1;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || pad.b)
	{
		staticPlayer = false;
		open = false;
	}


	return true;
}

// Update: draw background
bool Shop::PostUpdate()
{
	bool ret = true;
	
	if (open)
	app->render->DrawTexture(shopTex, 30, 80, NULL,2);
	if (selectorActive == true && open) 
	{
		int posY;
		if (shopY == 1)
			posY = shopY * 56;
		else if (shopY == 2)
			posY = shopY * 45;
		else if (shopY == 3)
			posY= shopY * 41;
		else if (shopY == 4)
			posY=shopY * 39;

		int posX;
		if (shopX == 1)
			posX = shopX * 16;
		else if (shopX == 2)
			posX = shopX * 31;
		else if (shopX == 3)
			posX = shopX * 36;
		else if (shopX == 4)
			posX = shopX * 39;


		getOwnedItems();
		app->render->DrawTexture(selectorTex, posX, posY, NULL);
	}
	return ret;
}


bool Shop::CleanUp()
{
	app->tex->UnLoad(shopTex);
	app->tex->UnLoad(selectorTex);
	app->tex->UnLoad(itemSold);

	return true;
}

void Shop::select()
{
	switch (itemPos) 
	{
	case WOOD_SWORD:
	{	
		WoodSword* woodsword = new WoodSword();
		buyItem(WOOD_SWORD,woodsword);
		
	}
	break;
	case STEEL_SWORD:
	{
		SteelSword* steelsword = new SteelSword();
		buyItem(STEEL_SWORD, steelsword);

		
	}
		break;
	case GOLD_SWORD:
	{
		GoldSword* goldsword = new GoldSword();
		buyItem(GOLD_SWORD, goldsword);


	}
		break;
	case FIRE_SWORD:
	{
		FireSword* firesword = new FireSword();
		buyItem(FIRE_SWORD, firesword);

	}
		break;
	case WOOD_PROTECTION:
	{
		WoodShield* woodshield = new WoodShield();
		buyItem(WOOD_PROTECTION, woodshield);


	}
		break;
	case STEEL_PROTECTION:
	{
		SteelShield* steelshield = new SteelShield();
		buyItem(STEEL_PROTECTION, steelshield);

	}
		break;
	case GOLD_PROTECTION:
	{
		GoldShield* goldshield = new GoldShield();
		buyItem(GOLD_PROTECTION, goldshield);

	}

		break;
	case FIRE_PROTECTION:	
	{
		FireShield* fireshield = new FireShield();
		buyItem(FIRE_PROTECTION, fireshield);

	}
		break;
	case WOOD_BOW:
	{
		WoodBow* woodbow = new WoodBow();
		buyItem(WOOD_BOW, woodbow);

	}

		break;
	case STEEL_BOW:
	{
		SteelBow* steelbow = new SteelBow();
		buyItem(STEEL_BOW, steelbow);

	}

		break;
	case GOLD_BOW:
	{
		GoldBow* goldbow = new GoldBow();
		buyItem(GOLD_BOW, goldbow);

	}

		break;
	case DIAMOND_BOW:
	{
		DiamondBow* diamondbow = new DiamondBow();
		buyItem(DIAMOND_BOW, diamondbow);

		
	}

		break;
	case WOOD_WAND:
	{
		WoodWand* woodwand = new WoodWand();
		buyItem(WOOD_WAND, woodwand);

	}

		break;
	case AMATIST_WAND:
	{
		AmatistWand* amatistwand = new AmatistWand();
		buyItem(AMATIST_WAND, amatistwand);

	}

		break;
	case RUBY_WAND:
	{
		RubyWand* rubywand = new RubyWand();
		buyItem(RUBY_WAND, rubywand);

	}

		break;
	case DIAMOND_WAND:
	{
		DiamondWand* diamondwand = new DiamondWand();
		buyItem(DIAMOND_WAND, diamondwand);

	}

		break;
	}


}

void Shop::getOwnedItems()
{
	if (open) {
		if (items[ItemType::WOOD_SWORD].selected == true) {
			app->render->DrawTexture(itemSold, 80, 170, NULL, 3);
		}

		if (items[ItemType::STEEL_SWORD].selected == true) {
			app->render->DrawTexture(itemSold, 215, 170, NULL, 3);
		}

		if (items[ItemType::GOLD_SWORD].selected == true) {
			app->render->DrawTexture(itemSold, 359, 170, NULL, 3);
		}

		if (items[ItemType::FIRE_SWORD].selected == true) {
			app->render->DrawTexture(itemSold, 497, 170, NULL, 3);
		}

		if (items[ItemType::WOOD_PROTECTION].selected == true) {
			app->render->DrawTexture(itemSold, 80, 269, NULL, 3);
		}

		if (items[ItemType::STEEL_PROTECTION].selected == true) {
			app->render->DrawTexture(itemSold, 215, 269, NULL, 3);
		}

		if (items[ItemType::GOLD_PROTECTION].selected == true) {
			app->render->DrawTexture(itemSold, 359, 269, NULL, 3);
		}

		if (items[ItemType::FIRE_PROTECTION].selected == true) {
			app->render->DrawTexture(itemSold, 497, 269, NULL, 3);
		}

		if (items[ItemType::WOOD_BOW].selected == true) {
			app->render->DrawTexture(itemSold, 80, 364, NULL, 3);
		}

		if (items[ItemType::STEEL_BOW].selected == true) {
			app->render->DrawTexture(itemSold, 215, 364, NULL, 3);
		}

		if (items[ItemType::GOLD_BOW].selected == true) {
			app->render->DrawTexture(itemSold, 359, 364, NULL, 3);
		}

		if (items[ItemType::DIAMOND_BOW].selected == true) {
			app->render->DrawTexture(itemSold, 497, 364, NULL, 3);
		}

		if (items[ItemType::WOOD_WAND].selected == true) {
			app->render->DrawTexture(itemSold, 80, 465, NULL, 3);
		}

		if (items[ItemType::AMATIST_WAND].selected == true) {
			app->render->DrawTexture(itemSold, 215, 465, NULL, 3);
		}

		if (items[ItemType::RUBY_WAND].selected == true) {
			app->render->DrawTexture(itemSold, 359, 465, NULL, 3);
		}

		if (items[ItemType::DIAMOND_WAND].selected == true) {
			app->render->DrawTexture(itemSold, 497, 465, NULL, 3);
		}
	}
}

void Shop::buyItem(ItemType itemType, Item *item)
{
	if (!app->scene->player->isItemInInventory(itemType) )
	{
		if (app->hud->coins >= item->price)
		{
			items[itemType].selected = true;
			app->scene->player->addItemToIventory(item);
			if (app->quests->quest2Taken)
			app->quests->quest2Done = true;
			app->audio->PlayFx(buyFx, 0);
		}
	}
}



