#ifndef __GOLDSHIELD_H__
#define __GOLDSHIELD_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Item.h"

struct SDL_Texture;
class Collider;

class GoldShield : public Item
{
public:

	GoldShield();

	// Destructor
	~GoldShield();

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



private:

};

#endif // __GoldShield_H__