#ifndef __NPC3_H__
#define __NPC3_H__

#include "Module.h"
#include "Render.h"
#include "Textures.h"
#include "Animation.h"
#include "Audio.h"
#include "Entity.h"

#include "Point.h"
#include "Defs.h"
#include "Log.h"

struct SDL_Texture;
class Collider;

class NPC3 : public Entity
{
public:
	// Constructor
	NPC3();
	// Destructor
	virtual ~NPC3();

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

	// NPC3 InitialPosition
	void NPC3InitialPosition();

	//iPoint position;
	iPoint posOrigin;
	iPoint posDestination;

	SDL_Texture* texNPC3;

	Animation* currentAnimation = nullptr;

	Animation idlAnim;
	Animation rightAnim;
	Animation leftAnim;

private:
	float speed = 1.0f;
	uint enemyDeathFx = 0;
	bool right = true;
	bool pause = false;
	int counter = 0;
	bool prova;
};

#endif // __NPC3_H__

