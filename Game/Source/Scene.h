#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class GuiControl;
class GuiButton;
class GuiSlider;
class GuiCheckBox;
class Player;
class Enemy;
class FlyingEnemy;
class ModuleParticles;

enum GameScene
{
	SCENE_NONE,
	SCENE_TOWN,
	SCENE_HOUSE1,
	SCENE_BSMITH,
	SCENE_INN
};

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// GuiClick
	bool OnGuiMouseClickEvent(GuiControl* control);

	// Change Scene
	void ChangeScene(GameScene nextScene);

	// Called before quitting
	bool CleanUp();

	void Pause();

	Player* player;
	Enemy* enemy;
	FlyingEnemy* flyingEnemy;
	ModuleParticles* particles;


	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	
	bool paused = false;
	bool pausedSettings = false;
	bool guiColliders = false;

	int lifesScene;
	int sceneCounterKey;
	int sceneCounterCheckpoint;
	int sceneCounterHeart;
	int sceneCounterPuzzle;
	bool restart = false;
	uint timer = 0;
private:
	// Scene Change
	GameScene currentScene;

	// Textures & Anim
	SDL_Texture* debugTex;
	
	SDL_Texture* pause;
	SDL_Texture* creditText = nullptr;
	SDL_Texture* clockText;

	Animation clockAnim;

	// GUI
	GuiButton* btnResume;
	GuiButton* btnSettings;
	GuiButton* btnBackIntro;
	GuiButton* btnExit;
	GuiButton* btnBackSettings;

	GuiButton* btnBack;

	GuiSlider* sliderMusicVolume;
	GuiSlider* sliderFxVolume;

	GuiCheckBox* checkBoxFullscreen;
	GuiCheckBox* checkBoxVSync;

	int volume;

	int cameraX;
	int cameraY;

	bool resumePause = false;
	bool settingsPause = false;
	bool backIntroPause = false;
	bool exitPause = false;

	int whiteFont = -1;
	char timerText[10] = { "\0" };
	
	int seconds = 0;
};

#endif // __SCENE_H__