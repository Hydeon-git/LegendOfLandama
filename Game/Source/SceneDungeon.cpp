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


SceneDungeon::SceneDungeon() : Module()
{
	name.Create("SceneDungeon");
}

SceneDungeon::~SceneDungeon()
{

}

bool SceneDungeon::Awake(pugi::xml_node& node)
{
	return true;
}

bool SceneDungeon::Start()
{
	bool ret = true;
	if (this->active == true)
	{
		LOG("Loading Scene Dungeon");
		// Font
		winText = app->tex->Load("Assets/Textures/scene_win.png");
		char lookupTable[] = { "! #$%&@()*+,-./0123456789:;<=>? ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
		whiteFont = app->font->Load("Assets/Textures/white_font.png", lookupTable, 1);

		// Map
		app->map->Enable();

		// Music
		app->audio->PlayMusic("Assets/Audio/Music/dungeon_music.ogg");

		// Entities
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		npc5 = (NPC5*)app->entityManager->CreateEntity(EntityType::NPC5);
		npc7 = (NPC7*)app->entityManager->CreateEntity(EntityType::NPC7);
		app->scene->player->dead = false;

		npc5->Start();
		npc7->Start();
		player->Start();		

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
	}
	return ret;
}

bool SceneDungeon::Update(float dt)
{
	if (!paused)
	{
		// Camera x
		if ((app->render->counter == 0 || player->godModeEnabled))
		{
			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
				&& !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
				&& !(app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
				&& !(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				&& !player->ThereIsLeftWall() 
				&& !player->ThereIsNPCLeft())
			{
				app->render->camera.x += 3.0f;
			}
				

			else if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
				&& !(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				&& !(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				&& !player->ThereIsRightWall()
				&& !player->ThereIsNPCRight())
			{
				app->render->camera.x -= 3.0f;
			}
				
		}
		// Camera y
		if ((app->render->counter == 0 || player->godModeEnabled))
		{

			if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) && player->position.y > 120 && player->position.y <= 400 && !player->ThereIsTopWall() && !player->ThereIsNPCUp() && !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)) app->render->camera.y += 3.0f;
			else if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) && player->position.y > 120 && player->position.y <= 400 && !player->ThereIsBottomWall() && !player->ThereIsNPCBelow() && !(app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && !(app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)) app->render->camera.y -= 3.0f;

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

	return true;
}

// Update: draw background
bool SceneDungeon::PostUpdate()
{
	bool ret = true;
	if (exit == true) ret = false;

	app->map->Draw();
	app->map->DrawColliders();
	
	return ret;
}

bool SceneDungeon::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		if (control->id == 1) app->fadeToBlack->FadeToBlk(this, app->sceneIntro, 30);
		else if (control->id == 2) exit = true;
	}
	default: break;
	}

	return true;
}

bool SceneDungeon::CleanUp()
{
	app->font->UnLoad(whiteFont);
	app->tex->UnLoad(winText);
	return true;
}