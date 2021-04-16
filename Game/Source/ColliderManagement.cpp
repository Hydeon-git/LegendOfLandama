#include "ColliderManagement.h"
#include "Render.h"
#include "EntityManager.h"
#include "Player.h"
#include "Audio.h"
#include "Render.h"
#include "App.h"

ColliderManagement::ColliderManagement() {}

ColliderManagement::~ColliderManagement() {}


bool ColliderManagement::Update(float dt, Player* player)
{
	ListItem<Collider*>* coll1 = collidersList.start;
	ListItem<Collider*>* coll2;

	while (coll1 != nullptr)
	{
		coll2 = coll1->next;

		while (coll2 != nullptr)
		{
			if (coll1->data->Intersects(&coll2->data->rect))
			{
				OnCollision(coll1->data, coll2->data, player);
			}
			else if (coll2->data->Intersects(&coll1->data->rect))
			{
				OnCollision(coll2->data, coll1->data, player);
			}
			coll2 = coll2->next;
		}
		coll1 = coll1->next;
	}

	return true;
}

void ColliderManagement::DrawColliders()
{
	Uint8 alpha = 80;
	ListItem<Collider*>* coll = collidersList.start;

	while (coll != nullptr)
	{
		switch (coll->data->type)
		{
		case Collider::Type::NONE:
			app->render->DrawRectangle(coll->data->rect, 255, 255, 255, alpha);
			break;
		case Collider::Type::PLAYER:
			app->render->DrawRectangle(coll->data->rect, 225, 0, 0, alpha);
			break;
		case Collider::Type::NPC1:
			app->render->DrawRectangle(coll->data->rect, 0, 0, 255, alpha);
			break;
		case Collider::Type::ENEMY:
			app->render->DrawRectangle(coll->data->rect, 0, 255, 0, alpha);
			break;
		case Collider::Type::ITEM:
			app->render->DrawRectangle(coll->data->rect, 255, 0, 255, alpha);
			break;
		}
		coll = coll->next;
	}
}

bool ColliderManagement::CleanUp()
{
	collidersList.Clear();

	return true;
}

Collider* ColliderManagement::AddCollider(SDL_Rect rect, Collider::Type type)
{
	Collider* ret = nullptr;

	ret = new Collider(rect, type);

	collidersList.Add(ret);

	return ret;
}

void ColliderManagement::RemoveCollider(Collider* collider)
{
	int index;
	ListItem<Collider*>* aux;

	index = collidersList.Find(collider);

	if (index != -1)
	{
		aux = collidersList.At(index);
		collidersList.Del(aux);
	}
}

void ColliderManagement::OnCollision(Collider* coll1, Collider* coll2, Player* player)
{
	/*if (coll1->type == Collider::Type::PLAYER && coll2->type == Collider::Type::ENEMY)
	{
		player->lifes--;
		app->audio->PlayFx(player->playerHurt);
		if (player->lifes > 0)
		{
			player->playerChangePos = true;
			app->render->ResetCam();
		}
		if (player->lifes == 0)
		{
			RemoveCollider(coll1);
			player->Dead();
		}
	}
	else if (coll2->type == Collider::Type::PLAYER && coll1->type == Collider::Type::ENEMY)
	{
		player->lifes--;
		app->audio->PlayFx(player->playerHurt);
		if (player->lifes > 0)
		{
			player->playerChangePos = true;
			app->render->ResetCam();
		}
		if (player->lifes == 0)
		{
			RemoveCollider(coll2);
			player->Dead();
		}
	}
	else if (coll1->type == Collider::Type::NPC1 && coll2->type == Collider::Type::ENEMY)
	{
		app->entityManager->EnemyLifes(coll2);
		RemoveCollider(coll1);
		app->sceneManager->score += 100;
	}
	else if (coll2->type == Collider::Type::SWORD && coll1->type == Collider::Type::ENEMY)
	{
		app->entityManager->EnemyLifes(coll1);
		RemoveCollider(coll2);
		app->sceneManager->score += 100;
	}
	else if (coll1->type == Collider::Type::PLAYER && coll2->type == Collider::Type::ITEM)
	{
		coll2->active = false;
	}
	else if (coll2->type == Collider::Type::PLAYER && coll1->type == Collider::Type::ITEM)
	{
		coll1->active = false;
	}*/
}

Collider::Collider(SDL_Rect rectangle, Type type) : rect(rectangle), type(type)
{}

void Collider::SetPos(int x, int y, SDL_Rect* rect)
{
	rect->x = x;
	rect->y = y;
}

bool Collider::Intersects(const SDL_Rect* r) const
{
	return (rect.x < r->x + r->w &&
		rect.x + rect.w > r->x &&
		rect.y < r->y + r->h &&
		rect.h + rect.y > r->y);
}