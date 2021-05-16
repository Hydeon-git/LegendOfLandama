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
class Enemy1;
class Enemy2;
class Enemy3;

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

	void Select();

	bool OnGuiMouseClickEvent(GuiControl* control);
	bool CleanUp() override;
	void Pause();
	void ChangeScene(DungeonScene nextScene);

	bool won = false;
	bool guiColliders = false;

	bool paused = false;

	DungeonScene currentScene;

	// Player
	Player* player;
	// NPCS
	NPC5* npc5;
	NPC7* npc7;

private:
	int whiteFont = -1;
	SDL_Texture* winText = nullptr;

	SDL_Texture* selectorTex = nullptr;
	
	bool pausedSettings = false;


	GuiButton* btnResume;
	GuiButton* btnSettings;
	GuiButton* btnBackIntro;
	GuiButton* btnExit;

	GuiButton* btnBack;

	GuiCheckBox* checkBoxFullscreen;

	SDL_Rect* fullscreenRect;
	int scale;

	int pos = 0;
	int posScaleY = 0, posScaleX = 0;

	int countChest = 0;
	int countSword = 0;
	// Enemy
	Enemy1* enemy1;
	Enemy2* enemy2;
	Enemy3* enemy3;
	
	// Textures
	SDL_Texture* texMenu;

	// GUI Buttons

};

#endif // __SCENEDUNGEON_H__