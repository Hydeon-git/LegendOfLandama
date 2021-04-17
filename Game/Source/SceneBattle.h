#ifndef __SCENE_BATTLE_H__
#define __SCENE_BATTLE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class GuiControl;
class GuiButton;
class Player;
class Enemy1;
class Enemy2;
class Enemy3;

class SceneBattle : public Module
{
public:
	SceneBattle();
	~SceneBattle();

	// Called when the module is activated
	bool Awake(pugi::xml_node& config) override;
	// Loads the necessary textures for the map background
	bool Start();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool CleanUp() override;

	bool posContinue = false;
	bool exit = false;
	bool startClicked = false;

	bool guiColliders = false;

	bool options = false;

	bool battleOn = false;

	Player* player;
	Enemy1* enemy1;
	Enemy2* enemy2;
	Enemy3* enemy3;

private:

	SString name;

	GuiButton* btnAttack;
	GuiButton* btnDefense;


	SDL_Texture* battletext = nullptr;

	SDL_Rect* fullscreenRect;

	int whiteFont = -1;

	int count = 0;
};


#endif //__SCENE_BATTLE_H__
