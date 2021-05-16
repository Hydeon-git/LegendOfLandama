#include "DialogSystem.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Shop.h"

#include "Font.h"
#include "Scene.h"
#include "SDL/include/SDL.h"
#include "Player.h"

#include <iostream>

DialogueSystem::DialogueSystem() {}

DialogueSystem::~DialogueSystem() {}

bool DialogueSystem::Start()
{
	LoadDialogue("dialogue_tree.xml");	
	currentNode = dialogueTrees[id]->dialogueNodes[0];	

	pointerText = app->tex->Load("Assets/Textures/pointer.png");


	return true;
}

bool DialogueSystem::Update(float dt)
{
	GamePad& pad = app->input->pads[0];
	if (app->scene->player != nullptr)
	{
		if (app->scene->player->dialogeOn)
		{
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.a == true) { Select(); }

			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || pad.up == true)
			{
				pos--;
				if (pos < 0 && currentNode->answersList.Count() == 2) pos = 1;
				else if (pos < 0 && currentNode->answersList.Count() == 3) pos = 2;
				else if (pos < 0 && currentNode->answersList.Count() == 1) pos = 0;
			}

			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN || pad.down == true)
			{
				pos++;
				if (pos > 2 && currentNode->answersList.Count() == 3) pos = 0;
				else if (pos > 1 && currentNode->answersList.Count() == 2) pos = 0;
				else if (pos > 0 && currentNode->answersList.Count() == 1) pos = 0;
			}

			if (input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || pad.b == true)
			{
				if (app->scene->currentScene == GameScene::SCENE_TOWN) id = 0;
				else if (app->scene->currentScene == GameScene::SCENE_HOUSE1)
				{
					if (!quest1In)
					{
						quest1In = true;
					}
					id = 2;
				}
				else if (app->scene->currentScene == GameScene::SCENE_BSMITH)
				{
					if (!quest3In)
					{
						quest3In = true;
					}
					id = 1;
				}

				else if (app->scene->currentScene == GameScene::SCENE_INN)
				{
					if (!quest2In)
					{
						quest2In = true;
					}
					id = 3;
				}

				currentNode = dialogueTrees[id]->dialogueNodes[0];
				playerInput = 9;
				app->scene->player->dialogeOn = false;

			}
		}
	}
	if (quest1In) 
	{
		if (quest1Counter <= 151) quest1Counter++;
	}
	if (quest2In)
	{
		if (quest2Counter <= 151) quest2Counter++;
	}
	if (quest3In)
	{
		if (quest3Counter <= 151) quest3Counter++;
	}
	return true;
}

bool DialogueSystem::PostUpdate()
{

	bool ret = true;

	if (quest1In)
	{ 
		if (quest1Counter < 150)
		{
			app->scene->quest1message = true;
		}else app->scene->quest1message = false;
	}
	if (quest2In)
	{
		if (quest2Counter < 150)
		{
			app->scene->quest2message = true;
		}
		else app->scene->quest2message = false;
	}
	if (quest3In)
	{
		if (quest3Counter < 150)
		{
			app->scene->quest3message = true;
		}
		else app->scene->quest3message = false;
	}



	if(app->scene->player != nullptr)
	{
		if (app->scene->player->dialogeOn)
		{

			
			//if (app->scene->player->ThereIsNPC() == 1) id = 1;
			if (app->scene->currentScene == GameScene::SCENE_BSMITH) id = 2;
			else if (app->scene->currentScene == GameScene::SCENE_TOWN) id = 3;
			else if (app->scene->currentScene == GameScene::SCENE_INN) id = 4;
			app->render->DrawRectangle({ 0, 520, 1280, 60 }, 0, 0, 0, 220, true, false);
			app->render->DrawRectangle({ 10, 530, 1260, 40 }, 100, 100, 200, 220, true, false);

			app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
			app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);

			char NPCdialogue[128] = { 0 };
			sprintf_s(NPCdialogue, 128, currentNode->text.c_str(), 56);
			app->font->DrawText(12, 178, app->scene->whiteFont, NPCdialogue);

			char response[128] = { 0 };
			for (int i = 0; i < currentNode->answersList.Count(); i++)
			{
				sprintf_s(response, 128, currentNode->answersList.At(i)->data.c_str(), 56);
				app->font->DrawText(15, 198 + 14 * i, app->scene->whiteFont, response);
			}

			if (pos == 0)
			{
				posScaleY = 196;
			}
			if (pos == 1)
			{
				posScaleY = 210;
			}
			if (pos == 2)
			{
				posScaleY = 224;
			}
			SDL_Rect rect;
			rect = { 0, 0, 16, 16 };
			app->render->DrawTexture(pointerText, 15, posScaleY, &rect, 1, false);
		}
	}

	return ret;
}

void DialogueSystem::Select()
{

	if (pos == 0)
	{
		if (app->scene->currentScene == GameScene::SCENE_TOWN) id = 0;
		else if (app->scene->currentScene == GameScene::SCENE_HOUSE1) id = 2;
		else if (app->scene->currentScene == GameScene::SCENE_BSMITH) id = 1;
		else if (app->scene->currentScene == GameScene::SCENE_INN) id = 3;

		checkPurchase();

		playerInput = 0;
		PerformDialogue(id);
		pos = 0;
	}
	else if (pos == 1)
	{
		if (app->scene->currentScene == GameScene::SCENE_TOWN) id = 0;
		else if (app->scene->currentScene == GameScene::SCENE_HOUSE1) id = 2;
		else if (app->scene->currentScene == GameScene::SCENE_BSMITH) id = 1;
		else if (app->scene->currentScene == GameScene::SCENE_INN) id = 3;

		playerInput = 1;
		PerformDialogue(id);
		pos = 0;
	}

	else if (pos == 2)
	{
		if (app->scene->currentScene == GameScene::SCENE_TOWN) id = 0;
		else if (app->scene->currentScene == GameScene::SCENE_HOUSE1) id = 2;
		else if (app->scene->currentScene == GameScene::SCENE_BSMITH) id = 1;
		else if (app->scene->currentScene == GameScene::SCENE_INN) id = 3;

		playerInput = 2;
		PerformDialogue(id);
		pos = 0;
	}
	

}

bool DialogueSystem::CleanUp()
{
	for (int i = 0; i < dialogueTrees.size(); i++)
	{
		for (int j = 0; j < dialogueTrees[i]->dialogueNodes.size(); j++) delete dialogueTrees[i]->dialogueNodes[j];

		dialogueTrees[i]->dialogueNodes.clear();
		delete dialogueTrees[i];
	}
	dialogueTrees.clear();
	app->tex->UnLoad(pointerText);
	return true;
}

void DialogueSystem::PerformDialogue(int treeId)
{
	if (playerInput >= 0 && playerInput < currentNode->dialogueOptions.size())
	{
		for (int i = 0; i < dialogueTrees[treeId]->dialogueNodes.size(); i++)
			if (currentNode->dialogueOptions[playerInput]->nextNode == dialogueTrees[treeId]->dialogueNodes[i]->nodeId)
			{
				currentNode = dialogueTrees[treeId]->dialogueNodes[i];
				break;
			}
	}

	//BlitDialog();
}

bool DialogueSystem::LoadDialogue(const char* file)
{
	pugi::xml_parse_result result = dialogues.load_file(file);

	bool ret = true;

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file, result.description());
		ret = false;
	}
	else
	{
		pugi::xml_node n = dialogues.first_child().child("NPC");

		for (n; n != NULL; n = n.next_sibling("NPC"))
		{
			DialogueTree* tree = new DialogueTree;
			tree->treeId = n.attribute("Id").as_int();
			LoadNodes(n, tree);
			dialogueTrees.push_back(tree);
		}
	}
	return true;
}

bool DialogueSystem::LoadNodes(pugi::xml_node& trees, DialogueTree* example)
{
	for (pugi::xml_node n = trees.child("node"); n != NULL; n = n.next_sibling("node"))
	{
		DialogueNode* node = new DialogueNode;
		node->text.assign(n.attribute("text").as_string());
		node->nodeId = n.attribute("id").as_int();
		LoadOptions(n, node);
		example->dialogueNodes.push_back(node);
	}

	return true;
}

bool DialogueSystem::LoadOptions(pugi::xml_node& response, DialogueNode* answers)
{
	for (pugi::xml_node option = response.child("dialogue"); option != NULL; option = option.next_sibling("dialogue"))
	{
		DialogueOption* selection = new DialogueOption;
		selection->text.assign(option.attribute("option").as_string());
		selection->nextNode = option.attribute("nextNode").as_int();
		answers->dialogueOptions.push_back(selection);
		answers->answersList.Add((option.attribute("option").as_string()));
	}

	return true;
}

void DialogueSystem::checkPurchase()
{
	for (int i = 0; i < currentNode->dialogueOptions.size(); ++i)
	{
		if (currentNode->dialogueOptions[i]->nextNode == 172)
		{
			app->scene->player->dialogeOn = false;
			app->shop->open = true;
			app->shop->staticPlayer = true;
			app->shop->selectorActive = true;
		}
	}

}
