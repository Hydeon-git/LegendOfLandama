#ifndef __FADETOBLACK_H__
#define __FADETOBLACK_H__

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define SCREEN_SIZE 3

#include "Module.h"
#include "SDL/include/SDL.h"

class FadeToBlack : public Module
{
public:

	FadeToBlack();
	virtual ~FadeToBlack();

	bool Awake();

	bool Start() override;


	bool Update(float dt) override;
	bool PostUpdate() override;

	bool FadeToBlk(Module* toDisable, Module* toEnable, float frames = 60);
	bool CleanUp() override;

	int animId = 0;

private:

	enum FadeStep
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	} currentStep = FadeStep::NONE;

	// A frame count system to handle the fade time and ratio
	Uint32 frameCount = 0;
	Uint32 maxFadeFrames = 0;

	// The rectangle of the screen, used to render the black rectangle
	SDL_Rect screenRect;
	SDL_Rect transit1;
	SDL_Rect transit2;
	SDL_Rect transit3;
	SDL_Rect transit4;
	SDL_Rect transit5{-SCREEN_WIDTH};
	SDL_Rect transit6{2*SCREEN_WIDTH};

	Module* moduleToEnable = nullptr;
	Module* moduleToDisable = nullptr;

	void transition1();
	void transition2();
	void transition3();
	void transition4();	
};
#endif // __FADETOBLACK_H__