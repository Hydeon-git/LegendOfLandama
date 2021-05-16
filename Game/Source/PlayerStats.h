#ifndef _PLAYERSTATS_H_
#define _PLAYERSTATS_H_

#include "Module.h"
#include "Animation.h"


struct SDL_Texture;


class PlayerStats : public Module
{
public:
	//Constructor
	PlayerStats();

	//Destructor
	~PlayerStats();

	bool Awake(pugi::xml_node& node);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void openBackPack();
	// Sound effects indices
	uint chooseFx = 0;
	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background

	bool statsIsOpen;


private:


	SDL_Texture* statsTex = nullptr;
	//SDL_Texture* markTex = nullptr;
	SDL_Texture* journalTex = nullptr;
	SDL_Texture* heroineTex = nullptr;
	SDL_Texture* knightTex = nullptr;
	SDL_Texture* mageTex = nullptr;


	void drawHealthHeroine();
	void drawHealthMage();
	void drawHealthKnight();

	int counterQuest1 = 0;
	int counterQuest2 = 0;
	int counterQuest3 = 0;

	uint questFx = 0;
	uint questDoneFx = 0;
};
#endif // !_PLAYERSTATS_H_
#pragma once
