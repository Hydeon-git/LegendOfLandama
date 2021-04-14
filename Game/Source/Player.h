#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Entity.h"

#define COLLIDER_GREEN 856
#define COLLIDER_RED 857
#define COLLIDER_BLUE 858
#define COLLIDER_YELLOW 859
#define COLLIDER_PINK 860
#define COLLIDER_GREY 861
#define COLLIDER_ORANGE 862

struct SDL_Texture;


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

	bool ThereIsTopWall();
	bool ThereIsBottomWall();
	bool ThereIsLeftWall();
	bool ThereIsRightWall();
	
	bool ThereIsChestBelow();
	bool ThereIsChestLeft();
	bool ThereIsChestRight();
	bool ThereAreSpikes();
	bool ThereIsEnemy();
	bool ThereIsFlyingEnemy();	
	bool TakeCheckpoint();	
	bool ThereIsDoor();
	bool LoseLifes();
	
	void CheckDoor();

	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation upAnim;
	Animation deathAnim;

	bool dead = false;
	bool win = false;
	bool godModeEnabled = false;

	
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

private:
	float speed = 1.0f;
	float fireBallSpeed = 3.0f;
	int playerHeight = 16;
	int playerWidth = 16;
	bool isJumping = false;


	SDL_Texture* texFireBall;
	uint playerDeathFx = 0;
	uint itemTakenFx = 0;
	uint checkpointFx = 0;
	uint chestFx = 0;
	uint heartFx = 0;
	uint fireFx = 0;
};

#endif // __PLAYER_H__