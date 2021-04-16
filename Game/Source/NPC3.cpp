#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Player.h"
#include "NPC3.h"
#include "ModuleParticles.h"
#include "Pathfinding.h"
#include "FadeToBlack.h"
#include "EntityManager.h"
#include "Defs.h"
#include "Log.h"

NPC3::NPC3() : Entity(EntityType::NPC3)
{
	name.Create("NPC3");
	position.x = 200;
	position.y = 100;

	//idlanim
	idlAnim.PushBack({ 0, 0, 21, 25 });
	idlAnim.PushBack({ 32, 0, 21, 25 });
	idlAnim.speed = 0.02f;

	//move right
	rightAnim.PushBack({ 0, 0, 21, 25 });
	rightAnim.PushBack({ 32, 0, 21, 25 });
	rightAnim.PushBack({ 64, 0, 21, 25 });
	rightAnim.PushBack({ 96, 0, 21, 25 });
	rightAnim.PushBack({ 127, 0, 22, 25 });
	rightAnim.PushBack({ 159, 0, 22, 25 });
	rightAnim.PushBack({ 191, 0, 22, 25 });
	rightAnim.PushBack({ 223, 0, 22, 25 });

	rightAnim.speed = 0.1f;

	//move left
	leftAnim.PushBack({ 248, 0, 22, 25 });
	leftAnim.PushBack({ 280, 0, 22, 25 });
	leftAnim.PushBack({ 312, 0, 22, 25 });
	leftAnim.PushBack({ 344, 0, 22, 25 });
	leftAnim.PushBack({ 376, 0, 21,25 });
	leftAnim.PushBack({ 408, 0, 21,25 });
	leftAnim.PushBack({ 440, 0, 21,25 });
	leftAnim.PushBack({ 472, 0, 21,25 });

	leftAnim.speed = 0.1f;

	currentAnimation = &idlAnim;
}

// Destructor
NPC3::~NPC3() {}

bool NPC3::Awake()
{
	LOG("Loading NPC3: Fisherman");
	bool ret = true;

	return ret;
}

bool NPC3::Start()
{
	if (this->active == true)
	{
		texNPC3 = app->tex->Load("Assets/Textures/npc3_character.png");
		currentAnimation = &idlAnim;
	}

	// Collider Load
	npc3Rect = { position.x, position.y, 16, 16 };
	if (npc3Collider == nullptr) npc3Collider = app->collision->AddCollider(npc3Rect, COLLIDER_NPC3, (Module*)this);

	return true;
}

bool NPC3::Update(float dt)
{
	currentAnimation = &idlAnim;
	currentAnimation->Update();

	// npc3Collider->SetPos(position.x, position.y);
	return true;
}

bool NPC3::PostUpdate()
{
	if ((this->active == true) && (app->scene->currentScene == GameScene::SCENE_HOUSE1))
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texNPC3, position.x, position.y, &rect);
	}
	return true;
}

bool NPC3::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(texNPC3);
	return true;
}

bool NPC3::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = false;
	if (!app->scene->player->godModeEnabled)
	{
		if (c1 == npc3Collider && c2->type == COLLIDER_PLAYER)
		{
			LOG("Hello Fisherman!");
			ret = true;
		}
	}
	return ret;
}



void NPC3::NPC3InitialPosition()
{
	position.x = 200;
	position.y = 100;
}
