#ifndef __SCENEDUNGEON_H__
#define __SCENEDUNGEON_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class GuiControl;
class GuiButton;
class GuiSlider;
class GuiCheckBox;
class Player;
class NPC5;
class NPC7;

enum class DungeonScene
{
	SCENE_NONE,
	SCENE_HALL,
	SCENE_MID,
	SCENE_BOSS
};

class SceneDungeon : public Module
{
public:

	SceneDungeon();

	// Destructor
	virtual ~SceneDungeon();

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

	bool won = false;
	bool guiColliders = false;

	DungeonScene currentScene;

private:
	GuiButton* btnRestart;
	GuiButton* btnExit;
	int whiteFont = -1;
	SDL_Texture* winText = nullptr;

	bool paused = false;
	bool exit = false;

	// Player
	Player* player;
	// NPCS
	NPC5* npc5;
	NPC7* npc7;
	
	// Textures
	SDL_Texture* texMenu;

	// GUI Buttons

};

#endif // __SCENEDUNGEON_H__