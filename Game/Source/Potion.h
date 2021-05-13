#ifndef __POTION_H__
#define __POTION_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Item.h"

struct SDL_Texture;
class Collider;

class Potion : public Item
{
public:

	Potion(iPoint pos);

	// Destructor
	~Potion();

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

	SDL_Texture* texPotion;
};

#endif // __Potion_H__