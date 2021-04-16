#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Collision.h"
#include "Module.h"
#include "Log.h"


Collision::Collision()
{
	name.Create("collision");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;	

	matrix[ColliderType::COLLIDER_PLAYER][ColliderType::COLLIDER_PLAYER] = false;
	matrix[ColliderType::COLLIDER_PLAYER][ColliderType::COLLIDER_ENEMY] = true;
	
	matrix[ColliderType::COLLIDER_ENEMY][ColliderType::COLLIDER_PLAYER] = true;
	matrix[ColliderType::COLLIDER_ENEMY][ColliderType::COLLIDER_ENEMY] = false;
}

// Destructor
Collision::~Collision()
{}

bool Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->toDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	
	// Calculate collisions
	Collider* c1 = nullptr;
	Collider* c2 = nullptr;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;
		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;
			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);
				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}
	
	return true;
}

// Called before render is available
bool Collision::Update(float dt) 
{
	return true;
}

void Collision::DebugDraw()
{
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case ColliderType::COLLIDER_NONE: // white
			app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, alpha);
			break;		
		case ColliderType::COLLIDER_PLAYER: // green
			app->render->DrawRectangle(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case ColliderType::COLLIDER_ENEMY: // red
			app->render->DrawRectangle(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		}
	}
}

// Called before quitting
bool Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* Collision::AddCollider(SDL_Rect rect, ColliderType type, Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	bool ret = false;

	if (rect.x <= r.x + r.w && rect.x + rect.w >= r.x && rect.y <= r.y + r.h && rect.h + rect.y >= r.y) ret = true;

	return ret;
}