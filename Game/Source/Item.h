#ifndef __ITEM_H__
#define __ITEM_H__


#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"

enum ItemType: int
{

	WOOD_SWORD,
	STEEL_SWORD,
	GOLD_SWORD,
	FIRE_SWORD,
	WOOD_PROTECTION,
	STEEL_PROTECTION,
	GOLD_PROTECTION,
	FIRE_PROTECTION,
	WOOD_BOW,
	STEEL_BOW,
	GOLD_BOW,
	DIAMOND_BOW,
	WOOD_WAND,
	AMATIST_WAND,
	RUBY_WAND,
	DIAMOND_WAND,
	POTION,
	COIN,
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

	virtual void useItem();

	virtual void damagedItem();


public:

	ItemType type;
	bool active = true;
	SString name;

	int price;
	iPoint position;
	bool renderable = false;
	SDL_Texture* texture;
};

#endif	
