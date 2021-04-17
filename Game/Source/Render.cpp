#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "Map.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "SceneWin.h"
#include "SceneLose.h"

#include "Defs.h"
#include "Log.h"

#define VSYNC true

Render::Render() : Module()
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		if (app->scene->player != nullptr)
		{
			camera.w = app->win->screenSurface->w;
			camera.h = app->win->screenSurface->h;
			camera.y = app->scene->player->position.y - (camera.h * 2) + 10;
		}
	}

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	scale = app->win->GetScale();
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	if (!app->scene->paused)
	{
		if (app->scene->player != nullptr)
		{
			if (app->scene->player->dead) DeadRestart();
		}
	}
	
	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

//restart values
void Render::RestartValues()
{
	if (app->scene->player != nullptr)
	{
		if (app->map->checkpointTaken && !app->sceneWin->won && !app->sceneLose->lost && !app->scene->restart)
		{
			app->scene->player->position.x = 938;
			app->scene->player->position.y = 171;

			app->render->camera.x = -588;
			app->render->camera.y = -99;
		}
		if (!app->map->checkpointTaken)
		{
			app->scene->player->position.x = 350;
			app->scene->player->position.y = 875;

			app->render->camera.x = app->scene->player->position.x - app->scene->player->position.x;
			app->render->camera.y = app->scene->player->position.y - (app->render->camera.h * 2) + 10-1440;
			
		}
		if (app->scene->restart)
		{
			app->scene->player->position.x = 350;
			app->scene->player->position.y = 875;

			app->render->camera.x = app->scene->player->position.x - app->scene->player->position.x;
			app->render->camera.y = app->scene->player->position.y - (app->render->camera.h * 2) + 10 - 1440;
			app->map->keyTaken = false;
			app->map->chestTaken = false;
			app->map->heartTaken = false;
			app->map->puzzleTaken = false;
			app->map->checkpointTaken = false;
			app->scene->player->lifes = 3;
			app->scene->player->counterKey = 0;
			app->scene->player->counterCheckpoint = 0;
			app->scene->player->counterHeart = 0;
			app->scene->player->counterPuzzle = 0;
			app->scene->restart = false;
			app->scene->timer = 0;
		}

		if (app->scene->player->dead || app->scene->player->win) app->render->camera.y = -2000;

		app->scene->enemy->active = true;
		app->scene->enemy->dead = false;
		app->scene->flyingEnemy->active = true;
		app->scene->flyingEnemy->dead = false;
		app->scene->enemy->EnemyInitialPosition();
		app->scene->flyingEnemy->FlyingEnemyInitialPosition();
		app->scene->player->deathAnim.Reset();
		counter = 0;
		app->scene->player->spiked = false;
		app->scene->player->dead = false;
		app->scene->player->godModeEnabled = false;
	}
}

void Render::DeadRestart()
{
	if (app->scene->player->currentAnimation == &app->scene->player->deathAnim)
	{
		if (counter > 100)
		{
			RestartValues();
			app->scene->player->dead = false;
		}
		else counter++;
	}
}

// L02: DONE 6: Implement a method to load the state, for now load camera's x and y
// Load Game State
bool Render::LoadState(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();
	return true;
}

// L02: DONE 8: Create a method to save the state of the renderer
// Save Game State
bool Render::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");
	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, int reducedScale, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;	

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * (scale / reducedScale);
	rect.y = (int)(camera.y * speed) + y * (scale / reducedScale);

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

	rect.w *= (scale / reducedScale);
	rect.h *= (scale / reducedScale);

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera) result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}