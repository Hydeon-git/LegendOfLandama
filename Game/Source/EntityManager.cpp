#include "EntityManager.h"

#include "Player.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "NPC1.h"
#include "ModuleParticles.h"
#include "App.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	if (!active) return true;

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* ret = nullptr;

	switch (type)
	{
		// L13: Create the corresponding type entity
		case EntityType::PLAYER: ret = new Player();  break;
		case EntityType::ENEMY: ret = new Enemy();  break;
		case EntityType::FLYING_ENEMY: ret = new FlyingEnemy();  break;
		case EntityType::PARTICLE: ret = new ModuleParticles();  break;
		case EntityType::NPC1: ret = new NPC1();  break;
		//case EntityType::ITEM: ret = new Item();  break;
		default: break;
	}

	// Created entities are added to the list
	if (ret != nullptr) entities.Add(ret);

	return ret;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem <Entity*>* item = entities.start;

	while (item != nullptr)
	{
		if (item->data == entity)
		{
			entities.Del(item);
			break;
		}
		item = item->next;
	}
}


bool EntityManager::PreUpdate()
{
	bool ret = true;

	ListItem<Entity*>* item = entities.start;

	while ((item != nullptr))
	{
		ret = item->data->PreUpdate();
		item = item->next;
	}

	return ret;
}

bool EntityManager::Update(float dt)
{
	if (!app->scene->paused)
	{
		for (unsigned int i = 0; i < entities.Count(); i++)
		{
			entities.At(i)->data->Update(dt);
		}
	}
	return true;
}

bool EntityManager::PostUpdate()
{
	ListItem<Entity*>* item = entities.start;

	while ((item != nullptr))
	{
		item->data->PostUpdate();
		item = item->next;
	}

	return true;
}
