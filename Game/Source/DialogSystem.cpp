#include "DialogSystem.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Font.h"
#include "Scene.h"
#include "SDL/include/SDL.h"
#include "Player.h"

DialogueSystem::DialogueSystem() {}

DialogueSystem::~DialogueSystem() {}

bool DialogueSystem::Start()
{
	LoadDialogue("dialogue_tree.xml");
	id = 1;
	currentNode = dialogueTrees[id]->dialogueNodes[0];	
	
	char lookupTable[] = { "! #$%& ()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[ ]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
	whiteFont = app->font->Load("Assets/Textures/white_font_mini.png", lookupTable, 1);
	return true;
}

bool DialogueSystem::Update(float dt)
{

	if (input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		playerInput = 0;
		PerformDialogue(id);
	}

	if (input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		playerInput = 1;
		PerformDialogue(id);
	}

	if (input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		playerInput = 2;
		PerformDialogue(id);
	}

	if (input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		id = 0;
		currentNode = dialogueTrees[id]->dialogueNodes[0];
		playerInput = 9;
		PerformDialogue(id);
	}



	return true;
}

bool DialogueSystem::PostUpdate()
{
	bool ret = true;

	if (app->scene->currentScene == GameScene::SCENE_TOWN && app->scene->player->dialogeOn)
	{
		app->render->DrawRectangle({ 0, 580, 1280, 140 }, 0, 0, 0, 220, true, false);
		app->render->DrawRectangle({ 10, 590, 1260, 120 }, 100, 100, 200, 220, true, false);

		char NPCdialogue[64] = { 0 };
		sprintf_s(NPCdialogue, 64, currentNode->text.c_str(), 56);
		app->font->DrawText(20, 200, whiteFont, NPCdialogue);

		char response[64] = { 0 };
		for (int i = 0; i < currentNode->answersList.Count(); i++)
		{
			sprintf_s(response, 64, currentNode->answersList.At(i)->data.c_str(), 56);
			app->font->DrawText(190, 200+12*i, whiteFont, response);
		}
	}



	return ret;
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