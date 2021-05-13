#ifndef __ITEM_H__
#define __ITEM_H__


#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"

enum class ItemType
{
	Potion,
	UNKNOWN
};

class Item
{
public:

	Item(ItemType type) : type(type), active(true) {}

	virtual bool PreUpdate()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}
	virtual bool PostUpdate()
	{
		return true;
	}

public:

	ItemType type;
	bool active = true;
	SString name;

	iPoint position;
	bool renderable = false;
	SDL_Texture* texture;
};

#endif	
