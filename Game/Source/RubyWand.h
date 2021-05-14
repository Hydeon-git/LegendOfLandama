#ifndef __RUBYWAND	_H__
#define __RUBYWAND_H__

#include "Module.h"
#include "Point.h"
#include "Animation.h"
#include "Item.h"

struct SDL_Texture;
class Collider;

class RubyWand : public Item
{
public:

	RubyWand();

	// Destructor
	~RubyWand();

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

#endif // __RubyWand_H__