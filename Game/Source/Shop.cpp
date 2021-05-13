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

		for (int i = 0; i < 16; i++) {
			items[i].selected = false;
		}
	}
	return ret;
}

bool Shop::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) { select(); }
	
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {

	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {

	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {

	}

	return true;
}

// Update: draw background
bool Shop::PostUpdate()
{
	bool ret = true;
	
	if (open)
	app->render->DrawTexture(shopTex, 30, 80, NULL,2);

	return ret;
}


bool Shop::CleanUp()
{
	app->font->UnLoad(shopFont);
	app->tex->UnLoad(shopTex);
	return true;
}

void Shop::select()
{



}
