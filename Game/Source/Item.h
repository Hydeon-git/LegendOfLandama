#ifndef __ITEM_H__
#define __ITEM_H__


#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"

enum class ItemType
{
	Potion,
	Coin,
	Wood_Sword,
	Steel_Sword,
	Gold_Sword,
	Fire_Sword,
	Wood_Shield,
	Steel_Shield,
	Gold_Shield,
	Fire_Shield,
	Wood_Bow,
	Steel_Bow,
	Gold_Bow,
	Diamond_Bow,
	Wood_Wand,
	Amatist_Wand,
	Ruby_Wand,
	Diamond_Wand,
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
