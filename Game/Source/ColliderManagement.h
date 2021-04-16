#pragma once

#include "Module.h"
#include "List.h"
#include "SDL/include/SDL.h"

class Collider
{
public:

	enum Type
	{
		NONE,
		PLAYER,
		NPC1,
		ENEMY,
		ITEM
	};

	Collider(SDL_Rect rectangle, Type type);

	void SetPos(int x, int y, SDL_Rect* rect);

	bool Intersects(const SDL_Rect* r) const;

	SDL_Rect rect;
	bool active;
	Type type;

};

class Player;

class ColliderManagement : public Module
{
public:

	ColliderManagement();
	~ColliderManagement();

	bool Update(float dt, Player* player);

	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, Collider::Type type);

	void RemoveCollider(Collider* collider);

	void DrawColliders();

	void OnCollision(Collider* coll1, Collider* coll2, Player* player);

private:

	List<Collider*> collidersList;
};