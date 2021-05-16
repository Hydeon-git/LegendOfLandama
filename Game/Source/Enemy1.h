#ifndef __ENEMY1_H__
#define __ENEMY1_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Entity.h"

struct SDL_Texture;
class Collider;

class Enemy1 : public Entity
{
public:

	Enemy1();

	// Destructor
	virtual ~Enemy1();

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


	void Enemy1InitialPosition();

	//iPoint position;
	iPoint posOrigin;
	iPoint posDestination;

	SDL_Texture* texEnemy1;

	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation rightAnim;
	Animation leftAnim;

	// Player Stats
	int enemy1Dmg = 15;
	int enemy1Health = 100;

private:
	float speed = 1.0f;
	uint enemyDeathFx = 0;
	bool right = true;
	bool pause = false;
	int counter = 0;
	bool prova;
};

#endif // __ENEMY1_H__