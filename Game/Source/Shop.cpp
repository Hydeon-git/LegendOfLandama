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
		char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
		shopFont = app->font->Load("Assets/Textures/white_font.png", lookupTable, 1);
		shopTex = app->tex->Load("Assets/Textures/shop.png");
		itemSold = app->tex->Load("Assets/Textures/shop_owned.png");
		selectorTex = app->tex->Load("Assets/Textures/pointer.png");
		selectorActive = false;
		shopX = 1;
		shopY = 1;


	}
	return ret;
}

bool Shop::Update(float dt)
{
	if (!selectorActive) return true;
	
	itemPos = (shopX + (4 * (shopY - 1)))-1;


	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) { select(); }
	

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) 
	{
		shopY--;
		if (shopY <= 0)shopY = 4;
	}


	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) 
	{
		shopY ++;
		if (shopY >=5)shopY = 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		shopX --;
		if (shopX <= 0) {
			shopX = 4;
		}

	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		shopX ++;
		if (shopX >= 5) {
			shopX = 1;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
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
	app->font->UnLoad(shopFont);
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
		items[WOOD_SWORD].selected = true;
		break;
	case STEEL_SWORD:
		items[STEEL_SWORD].selected = true;

		break;
	case GOLD_SWORD:
		items[GOLD_SWORD].selected = true;

		break;
	case FIRE_SWORD:
		items[FIRE_SWORD].selected = true;

		break;
	case WOOD_PROTECTION:
		items[WOOD_PROTECTION].selected = true;

		break;
	case STEEL_PROTECTION:
		items[STEEL_PROTECTION].selected = true;

		break;
	case GOLD_PROTECTION:
		items[GOLD_PROTECTION].selected = true;

		break;
	case FIRE_PROTECTION:		
		items[FIRE_PROTECTION].selected = true;

		break;
	case WOOD_BOW:
		items[WOOD_BOW].selected = true;

		break;
	case STEEL_BOW:
		items[STEEL_BOW].selected = true;

		break;
	case GOLD_BOW:
		items[GOLD_BOW].selected = true;

		break;
	case DIAMOND_BOW:
		items[DIAMOND_BOW].selected = true;

		break;
	case WOOD_WAND:
		items[WOOD_WAND].selected = true;

		break;
	case AMATIST_WAND:
		items[AMATIST_WAND].selected = true;

		break;
	case RUBY_WAND:
		items[RUBY_WAND].selected = true;

		break;
	case DIAMOND_WAND:
		items[DIAMOND_WAND].selected = true;

		break;
	}


}

void Shop::getOwnedItems()
{
	if (open) {
		if (items[WOOD_SWORD].selected == true) {
			app->render->DrawTexture(itemSold, 80, 170, NULL, 3);
		}

		if (items[STEEL_SWORD].selected == true) {
			app->render->DrawTexture(itemSold, 215, 170, NULL, 3);
		}

		if (items[GOLD_SWORD].selected == true) {
			app->render->DrawTexture(itemSold, 359, 170, NULL, 3);
		}

		if (items[FIRE_SWORD].selected == true) {
			app->render->DrawTexture(itemSold, 497, 170, NULL, 3);
		}

		if (items[WOOD_PROTECTION].selected == true) {
			app->render->DrawTexture(itemSold, 80, 269, NULL, 3);
		}

		if (items[STEEL_PROTECTION].selected == true) {
			app->render->DrawTexture(itemSold, 215, 269, NULL, 3);
		}

		if (items[GOLD_PROTECTION].selected == true) {
			app->render->DrawTexture(itemSold, 359, 269, NULL, 3);
		}

		if (items[FIRE_PROTECTION].selected == true) {
			app->render->DrawTexture(itemSold, 497, 269, NULL, 3);
		}

		if (items[WOOD_BOW].selected == true) {
			app->render->DrawTexture(itemSold, 80, 364, NULL, 3);
		}

		if (items[STEEL_BOW].selected == true) {
			app->render->DrawTexture(itemSold, 215, 364, NULL, 3);
		}

		if (items[GOLD_BOW].selected == true) {
			app->render->DrawTexture(itemSold, 359, 364, NULL, 3);
		}

		if (items[DIAMOND_BOW].selected == true) {
			app->render->DrawTexture(itemSold, 497, 364, NULL, 3);
		}

		if (items[WOOD_WAND].selected == true) {
			app->render->DrawTexture(itemSold, 80, 465, NULL, 3);
		}

		if (items[AMATIST_WAND].selected == true) {
			app->render->DrawTexture(itemSold, 215, 465, NULL, 3);
		}

		if (items[RUBY_WAND].selected == true) {
			app->render->DrawTexture(itemSold, 359, 465, NULL, 3);
		}

		if (items[DIAMOND_WAND].selected == true) {
			app->render->DrawTexture(itemSold, 497, 465, NULL, 3);
		}
	}
}
