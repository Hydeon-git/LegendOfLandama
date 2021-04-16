#ifndef __Collision_H__
#define __Collision_H__

#define MAX_COLLIDERS 3000

#include "Module.h"
#include "SDL\include\SDL_rect.h"

enum ColliderType
{
	COLLIDER_NONE = -1,	
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool toDelete = false;
	ColliderType type;
	Module* callback = nullptr;

	Collider(SDL_Rect rectangle, ColliderType type, Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class Collision : public Module
{
public:

	Collision();
	~Collision();

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	Collider* AddCollider(SDL_Rect rect, ColliderType type, Module* callback = nullptr);
	void DebugDraw();
	SDL_Rect rect;

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
};

#endif // __Collision_H__