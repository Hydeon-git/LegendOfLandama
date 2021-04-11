#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Entity.h"

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

	bool ThereIsGround();
	bool ThereIsLeftWall();
	bool ThereIsRightWall();
	
	bool ThereIsChestBelow();
	bool ThereIsChestLeft();
	bool ThereIsChestRight();
	bool ThereAreSpikes();
	bool ThereIsEnemy();
	bool ThereIsFlyingEnemy();
	bool TakeKey();
	bool TakePuzzle();
	bool TakeCheckpoint();
	bool TakeHeart();
	bool ThereIsDoor();
	bool LoseLifes();
	

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

private:
	float speed = 1.0f;
	float fireBallSpeed = 3.0f;
	int playerHeight = 85;
	int playerWidth = 64;
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