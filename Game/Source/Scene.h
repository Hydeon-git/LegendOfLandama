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
class NPC1;
class NPC2;
class NPC3; 
class NPC4;
class NPC5;
class NPC7;
class Enemy1;
class Enemy2;
class Enemy3;

enum class GameScene
{
	SCENE_NONE,
	SCENE_TOWN,
	SCENE_HOUSE1,
	SCENE_BSMITH,
	SCENE_INN,
	SCENE_ENTRYDUNGEON
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

	void Select();

	// GuiClick
	bool OnGuiMouseClickEvent(GuiControl* control);

	// Change Scene
	void ChangeScene(GameScene nextScene);	
	GameScene currentScene;

	// Called before quitting
	bool CleanUp();

	void Pause();

	Player* player;
	Enemy* enemy;
	FlyingEnemy* flyingEnemy;
	ModuleParticles* particles;
	NPC1* npc1;
	NPC2* npc2;
	NPC3* npc3;
	NPC4* npc4;
	NPC5* npc5;
	NPC7* npc7;
	Enemy1* enemy1;
	Enemy2* enemy2;
	Enemy3* enemy3;

	int whiteFont = -1;

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
	bool stop = false;
	bool enemyMoving = false;
	bool houseClosed = false;
	
	bool mageTxt = false;
	bool mageTxt2 = false;
	bool knightTxt = false;
	bool knightTxt2 = false;
	bool mageTkn = false;
	bool knightTkn = false;
	bool fromDungeon = false;
	bool forest = false;


	int quest1message = false;
	int quest2message = false;
	int quest3message = false;
private:

	// Textures & Anim
	SDL_Texture* debugTex;
	
	SDL_Texture* texMenu;

	SDL_Texture* pause;
	SDL_Texture* creditText = nullptr;
	SDL_Texture* no;

	SDL_Texture* selectorTex = nullptr;

	Animation clockAnim;

	// GUI
	SDL_Rect* fullscreenRect;
	int scale;
	GuiButton* btnResume;
	GuiButton* btnSettings;
	GuiButton* btnBackIntro;
	GuiButton* btnExit;

	GuiButton* btnBack;

	GuiSlider* sliderMusicVolume;
	GuiSlider* sliderFxVolume;

	GuiCheckBox* checkBoxFullscreen;
	GuiCheckBox* checkBoxVSync;

	int volume;

	int cameraX;
	int cameraY;

	// Detecting house
	// 0 - None
	// 1 - House1
	// 2 - Blacksmith
	// 3 - Inn
	int house = 0;

	bool resumePause = false;
	bool settingsPause = false;
	bool backIntroPause = false;
	bool exitPause = false;
	bool knokDone = false;	
		
	char timerText[10] = { "\0" };
	
	int seconds = 0;
	int pos = 0;
	int posScaleY = 0, posScaleX = 0;
	uint doorOpenFx = 0;
	uint doorCloseFx = 0;
	uint doorKnokFx = 0;

	int count = 0;
	int count2 = 0;
	int count3 = 0;

};

#endif // __SCENE_H__