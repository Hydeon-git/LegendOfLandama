#ifndef __NPC5_H__
#define __NPC5_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Entity.h"

struct SDL_Texture;
class Collider;

class NPC5 : public Entity
{
public:

	NPC5();

	// Destructor
	virtual ~NPC5();

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


	void NPC5InitialPosition();

	//iPoint position;
	iPoint posOrigin;
	iPoint posDestination;

	SDL_Texture* texNPC5;

	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation rightAnim;
	Animation leftAnim;
	Animation upAnim;

	// NPC5 Mage Stats
	int mageDmg = 10;
	int mageHealth = 50;
	int mageMagicDmg = 40;

	

private:
	float speed = 1.0f;
	uint enemyDeathFx = 0;
	bool right = true;
	bool pause = false;
	int counter = 0;
	bool prova;

};

#endif // __NPC5_H__