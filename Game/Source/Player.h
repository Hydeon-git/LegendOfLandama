#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Entity.h"
#include "Item.h"	

#define COLLIDER_GREEN 856
#define COLLIDER_RED 857
#define COLLIDER_BLUE 858
#define COLLIDER_YELLOW 859
#define COLLIDER_PINK 860
#define COLLIDER_GREY 861
#define COLLIDER_ORANGE 862
#define COLLIDER_CIAN 863

#define COLLIDER_GREEN_HOUSE 769
#define COLLIDER_RED_HOUSE 770
#define COLLIDER_CIAN_HOUSE 776

#define COLLIDER_GREEN_FOREST 1345
#define COLLIDER_RED_FOREST 1346

#define COLLIDER_GREEN_DUNGEON 1025
#define COLLIDER_RED_DUNGEON 1026
#define COLLIDER_BLUE_DUNGEON 1027
#define COLLIDER_YELLOW_DUNGEON 1028
#define COLLIDER_PINK_DUNGEON 1029
#define COLLIDER_GREY_DUNGEON 1030
#define COLLIDER_ORANGE_DUNGEON 1031
#define COLLIDER_CIAN_DUNGEON 1032
#define COLLIDER_BEIGE_DUNGEON 1033
#define COLLIDER_GREENER_DUNGEON 1034
#define COLLIDER_BLACK_DUNGEON 1035
#define COLLIDER_BROWN_DUNGEON 1036

struct SDL_Texture;
class Collider;

class Player : public Entity
{
public:

	Player();

	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Called On Collision
	//bool OnCollision(Collider* c1, Collider* c2);

	bool ThereIsTopWall();
	bool ThereIsBottomWall();
	bool ThereIsLeftWall();
	bool ThereIsRightWall();
	int ThereIsNPC();
	bool ThereIsNPCBelow();
	bool ThereIsNPCUp();
	bool ThereIsNPCLeft();
	bool ThereIsNPCRight();
	bool ThereIsLimit();
	bool ThereIsHouseClosed();
	bool ThereIsEnemy();
	bool ThereIsFlyingEnemy();
	bool DungeonDoorOpen();
	bool DungeonFloorUp();
	bool ThereIsDoor();
	bool OpenChest();
	bool OpenLever1();
	bool OpenLever2();
	bool OpenLever3();
	bool OpenLeverFinal();
	bool TakeBlueKey();
	bool TakeRedKey();
	bool TakeGreenKey();
	bool TakeYellowKey();
	bool LoseLifes();

	void CheckDoor();
	void CheckHouseDoor();


	bool doorTaked;
	bool doorTaked2;

	bool kDoorTaked;
	bool kDoorTaked2;

	int posMoved = 0;
	int kposMoved = 0;
	bool doIt = false;

	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation upAnim;
	Animation deathAnim;

	bool dead = false;
	bool win = false;
	bool godModeEnabled = false;
	bool dialogeOn = false;
	bool onBattle = false;
	int lifes; //player lifes
	bool spiked; //if player lose a life

	int shotMaxCountdown = 20;
	int shotCountdown = 0;
	SDL_Texture* texPlayer;

	int counterKey;
	int counterPuzzle;
	int counterCheckpoint;
	int counterHeart;
	bool loaded = false;
	bool house = false;
	int door = 0;
	int houseDoor = 0;


	bool spikes1Down = false;
	bool spikes2Down = false;
	bool spikes3Down = false;

	//inventory
	Item* inventory[13];
	void addItemToIventory(Item* item);
	void drawPlayerInventory();
	bool checkInventoryPos(int pos);
	bool isItemInInventory(ItemType itemtype);

	int lastPosX[25] = { position.x, position.x, position.x, position.x,position.x, position.x, position.x, position.x, position.x,position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x };
	int lastPosY[25] = { position.y, position.y, position.y, position.y,position.y, position.y, position.y, position.y, position.y,position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y };

	int klastPosX[50] = { position.x, position.x, position.x, position.x,position.x, position.x, position.x, position.x, position.x,position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x , position.x, position.x, position.x, position.x, position.x, position.x, position.x, position.x };
	int klastPosY[50] = { position.y, position.y, position.y, position.y,position.y, position.y, position.y, position.y, position.y,position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y , position.y, position.y, position.y, position.y, position.y, position.y, position.y, position.y };

	int coins;

	int lastPositionX;
	int lastPositionY;
	int lastPositionX2;
	int lastPositionY2;

	int klastPositionX;
	int klastPositionY;
	int klastPositionX2;
	int klastPositionY2;

	

	// Player Stats
	int playerDmg = 250;
	int playerHealth = 200;
	int playerMagicDmg = 15;

	bool battleDoneMage = false;
	bool battleDoneKnight = false;

	bool button1Done = false;
	bool button2Done = false;
	bool lever1Done = false;
	bool lever2Done = false;
	bool lever3Done = false;
	bool leverFinalDone = false;
	bool keyRedDone = false;
	bool keyBlueDone = false;
	bool keyGreenDone = false;
	bool keyYellowDone = false;

private:
	float speed = 1.0f;
	float fireBallSpeed = 3.0f;
	int playerHeight = 16;
	int playerWidth = 16;
	bool isJumping = false;

private:
	SDL_Texture* texFireBall;
	uint playerDeathFx = 0;
	uint itemTakenFx = 0;
	uint keyTakenFx = 0;
	uint chestFx = 0;
	uint heartFx = 0;
	uint fireFx = 0;

	uint leverFx = 0;
	uint leverErrorFx = 0;
	uint puzzleSolvedFx = 0;
	uint buttonFx = 0;
	uint talkFx = 0;

};

#endif // __PLAYER_H__