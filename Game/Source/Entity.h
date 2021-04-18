#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"
#include "PugiXml/src/pugixml.hpp"

enum class EntityType
{
	PLAYER,
	ENEMY,
	FLYING_ENEMY,
	PARTICLE,
	NPC1,
	NPC2,
	NPC3,
	NPC4,
	NPC5,
	Enemy1,
	Enemy2,
	Enemy3,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

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

	EntityType type;
	bool active = true;
	SString name;

	iPoint position;
	bool renderable = false;
	SDL_Texture* texture;
};

#endif // __ENTITY_H__