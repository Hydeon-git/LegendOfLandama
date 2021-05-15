#ifndef _QUESTS_H_
#define _QUESTS_H_

#include "Module.h"
#include "Animation.h"


struct SDL_Texture;


class Quests : public Module
{
public:
	//Constructor
	Quests();

	//Destructor
	~Quests();

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

	bool questsIsOpen;

	bool quest1Taken = false;
	bool quest2Taken = false;
	bool quest3Taken = false;

	bool quest1Done = false;
	bool quest2Done = false;
	bool quest3Done = false;

private:


	SDL_Texture* questsTex = nullptr;
	SDL_Texture* markTex = nullptr;
	SDL_Texture* journalTex = nullptr;

	void drawMark1();
	void drawMark2();
	void drawMark3();

	int counterQuest1 = 0;
	int counterQuest2 = 0;
	int counterQuest3 = 0;

	uint questFx = 0;
	uint questDoneFx = 0;
};
#endif // !_QUESTS_H_
