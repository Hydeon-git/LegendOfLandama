#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Entity.h"

struct SDL_Texture;


class Enemy : public Entity
{
public:

	Enemy();

	// Destructor
	virtual ~Enemy();

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

	bool ThereIsGroundLeft();
	bool ThereIsGroundRight();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool IsDead();

	void EnemyInitialPosition();

	//iPoint position;
	iPoint posOrigin;
	iPoint posDestination;

	SDL_Texture* texEnemy;

	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation deathAnim;

	bool dead = false;

private:
	float speed = 2.0f;
	uint enemyDeathFx = 0;

};

#endif // __ENEMY_H__