#include "FadeToBlack.h"


#include "App.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "easings.h"

#include "Log.h"

FadeToBlack::FadeToBlack()
{

	name.Create("fadeToBlack");

}


FadeToBlack::~FadeToBlack()
{

}


bool FadeToBlack::Awake()
{
	LOG("Loading FadeToBlack");
	bool ret = true;

	return ret;
}


bool FadeToBlack::Start()
{
	if (this->active == true)
	{
		uint w, h;
		app->win->GetWindowSize(w, h);

		screenRect = {0, 0, (int)w * (int)app->win->GetScale(), (int)h * (int)app->win->GetScale()};

		SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	}
	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == FadeStep::NONE) return true;

	if (currentStep == FadeStep::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = FadeStep::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;

		currentStep = FadeStep::NONE;		
	}

	return true;
}

bool FadeToBlack::PostUpdate()
{
	if (currentStep == FadeStep::NONE) return true;

	if (animId == 0) transition1();	
	else if (animId == 1) transition2();
	else if (animId == 2) transition3();	
	else if (animId == 3) transition4();	
	else if (animId > 3) animId == 0;
	

	return true;
}


bool FadeToBlack::FadeToBlk(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;
		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;
		ret = true;
	}

	return ret;
}

bool FadeToBlack::CleanUp()
{
	return true;
}

void FadeToBlack::transition1() //Left upper edge of the screen square covers all the screen and goes backwards
{
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(app->render->renderer, &transit1);
	transit1.x = 0;
	transit1.y = 0;
	transit1.h = EaseBounceInOut(frameCount, SCREEN_HEIGHT / 8, SCREEN_HEIGHT, 45);
	transit1.w = EaseBounceInOut(frameCount, SCREEN_WIDTH / 8, SCREEN_WIDTH, 45);
}

void FadeToBlack::transition2() // Fade to black
{
	float fadeRatio = (float)frameCount / (float)maxFadeFrames;
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);
}

void FadeToBlack::transition3() // All four edges merge overlapping the screen
{
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(app->render->renderer, &transit1);
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(app->render->renderer, &transit2);
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(app->render->renderer, &transit3);
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(app->render->renderer, &transit4);

	transit1.x = 0;
	transit1.y = 0;
	transit1.h = EaseBounceInOut(frameCount, SCREEN_HEIGHT / 8, SCREEN_HEIGHT, 45);
	transit1.w = EaseBounceInOut(frameCount, SCREEN_WIDTH / 8, SCREEN_WIDTH, 45);

	transit2.x = 384 * SCREEN_SIZE;
	transit2.y = 240 * SCREEN_SIZE;
	transit2.h = EaseBounceInOut(frameCount, SCREEN_HEIGHT / 8, -SCREEN_HEIGHT, 45);
	transit2.w = EaseBounceInOut(frameCount, SCREEN_WIDTH / 8, -SCREEN_WIDTH, 45);

	transit3.x = 0 * SCREEN_SIZE;
	transit3.y = 240 * SCREEN_SIZE;
	transit3.h = EaseBounceInOut(frameCount, SCREEN_HEIGHT / 8, -SCREEN_HEIGHT, 45);
	transit3.w = EaseBounceInOut(frameCount, -SCREEN_WIDTH / 8, SCREEN_WIDTH, 45);

	transit4.x = 384 * SCREEN_SIZE;
	transit4.y = 0 * SCREEN_SIZE;
	transit4.h = EaseBounceInOut(frameCount, -SCREEN_HEIGHT / 8, SCREEN_HEIGHT, 45);
	transit4.w = EaseBounceInOut(frameCount, SCREEN_WIDTH / 8, -SCREEN_WIDTH, 45);

}

void FadeToBlack::transition4() //Trying to make two rectangles move to screen centre and get back
{
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(app->render->renderer, &transit5);
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(app->render->renderer, &transit6);

	transit5.h = SCREEN_HEIGHT / 2 * SCREEN_SIZE;
	transit5.w = EaseLinearInOut(frameCount, 0, SCREEN_WIDTH * SCREEN_SIZE, 45);
	transit5.x = 0 * SCREEN_SIZE;
	transit5.y = 0 * SCREEN_SIZE;

	transit6.h = SCREEN_HEIGHT / 2 * SCREEN_SIZE;
	transit6.w = EaseLinearOut(frameCount, SCREEN_WIDTH * SCREEN_SIZE, -SCREEN_WIDTH * SCREEN_SIZE, 45);
	transit6.x = SCREEN_WIDTH * SCREEN_SIZE;
	transit6.y = SCREEN_HEIGHT / 2 * SCREEN_SIZE;

}
