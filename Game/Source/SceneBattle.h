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
class NPC5;
class NPC7;

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

	// Attack Functions
	void EnemyAttack();

	bool posContinue = false;
	bool exit = false;
	bool startClicked = false;

	bool guiColliders = false;

	bool options = false;

	bool battleOn = false;
	bool battleEnd = false;
	Player* player;
	Enemy1* enemy1;
	Enemy2* enemy2;
	Enemy3* enemy3;
	NPC5* npc5;
	NPC7* npc7;

	bool enemy1Dead = false;
	bool enemy2Dead = false;
	bool enemy3Dead = false;

	bool playerDead = false;
	bool mageDead = false;
	bool knightDead = false;
	int enemiesAlive = 3;

	bool playerRevive = false;
	bool mageRevive = false;
	bool knightRevive = false;

	int pos = 0;

	void select();



	bool Keyboard(GuiControl* control);

private:

	SString name;

	GuiButton* btnHeroine;
	GuiButton* btnMage;
	GuiButton* btnKnight;
	GuiButton* btnAttack;
	GuiButton* btnMagic;
	GuiButton* btnDefense;
	GuiButton* btnEnemy1;
	GuiButton* btnEnemy2;
	GuiButton* btnEnemy3;

	SDL_Texture* battletext = nullptr;
	SDL_Texture* selectorTex = nullptr;


	SDL_Rect* fullscreenRect;

	bool heroine = false;
	bool mage = false;
	bool knight = false;
	bool attack = false;
	bool magic = false;

	bool defense = false;
	bool hDefense = false;
	bool mDefense = false;
	bool kDefense = false;

	int posY = 0, posX = 0;
	int posScaleY = 0, posScaleX = 0;

	bool clicked = false;

	int whiteFont = -1;

	int goldFont = -1;

	int count = 0;

	// Enemy counter
	int enemyCounter;

	// Characters turns counters
	int heroineCounter;
	int mageCounter;
	int knightCounter;

	// Player HP display
	char heroineHpText[10] = { "\0" };
	char mageHpText[10] = { "\0" };
	char knightHpText[10] = { "\0" };

	// Enemy HP display
	char enemy1HpText[10] = { "\0" };
	char enemy2HpText[10] = { "\0" };
	char enemy3HpText[10] = { "\0" };

	uint hitEnemyFx = 0;
	uint magicEnemyFx = 0;

};


#endif //__SCENE_BATTLE_H__
