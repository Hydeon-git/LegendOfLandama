#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Map.h"
#include "ModuleParticles.h"
#include "Font.h"
#include "EntityManager.h"
#include "FadeToBlack.h"
#include "SceneIntro.h"
#include "SceneWin.h"
#include "SceneLose.h"
#include "SceneBattle.h"
#include "SceneDungeon.h"
#include "PathFinding.h"
#include "Player.h"
#include "Shop.h"
#include "DialogSystem.h"
#include "Defs.h"
#include "Log.h"
#include "HUD.h"
#include <iostream>
#include <sstream>

// L07: DONE 3: Measure the amount of ms that takes to execute:
// App constructor, Awake, Start and CleanUp
// LOG the result

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	win = new Window();
	input = new Input();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	dialogueSystem = new DialogueSystem(input, render, tex);
	map = new Map();
	scene = new Scene();
	sceneDungeon = new SceneDungeon();
	sceneBattle = new SceneBattle();
	sceneIntro = new SceneIntro();
	sceneWin = new SceneWin();
	sceneLose = new SceneLose();
	entityManager = new EntityManager();
	fadeToBlack = new FadeToBlack();
	pathfinding = new PathFinding();
	font = new Font();
	shop = new Shop();
	hud = new HUD();



	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win, true);
	AddModule(input, true);
	AddModule(tex, true);
	AddModule(audio, true);
	AddModule(sceneIntro, true);
	AddModule(sceneWin, false);
	AddModule(sceneLose, false);
	AddModule(sceneBattle, false);
	AddModule(scene, false);
	AddModule(sceneDungeon, false);
	AddModule(map, false);
	AddModule(dialogueSystem, true);
	AddModule(entityManager, true);
	AddModule(font, true);
	AddModule(pathfinding, true);
	AddModule(fadeToBlack, true);
	AddModule(shop, false);
	AddModule(hud, false);
	// Render last to swap buffer
	AddModule(render, true);

	PERF_PEEK(ptimer);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module, bool active)
{
	module->Init(active);
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());

		// L08: DONE 1: Read from config file your framerate cap
		int cap = configApp.child("fps").attribute("capMax").as_int();
		int capMin = configApp.child("fps").attribute("capMin").as_int();

		if (cap > 0) cappedMs = 1000 / cap;
		if (capMin > 0) cappedMsMin = 1000 / capMin;
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}
	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(ptimer);

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	PERF_START(ptimer);

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true) ret = false;

	if(ret == true) ret = PreUpdate();

	if(ret == true) ret = DoUpdate();

	if(ret == true) ret = PostUpdate();

	FinishUpdate();

	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;

	// L08: DONE 4: Calculate the dt: differential time since last frame
	dt = frameTime.ReadSec();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L02: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	// L07: DONE 4: Framerate calculations
	// Amount of frames since startup
	// Amount of time since game start (use a low resolution timer)
	// Average FPS for the whole game life
	// Amount of ms took the last update
	// Amount of frames during the last second
	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float averageFps = float(frameCount) / startupTime.ReadSec();
	float secondsSinceStartup = startupTime.ReadSec();
	uint32 lastFrameMs = frameTime.Read();
	uint32 framesOnLastUpdate = prevLastSecFrameCount;

	static char title[256];
	if (!vSync) sprintf_s(title, 256, "FPS: %i // Avg. FPS: %.2f // Last-frame MS: %02u // Vsync : off", framesOnLastUpdate, averageFps, lastFrameMs);
	else sprintf_s(title, 256, "FPS: %i // Avg. FPS: %.2f // Last-frame MS: %02u // Vsync : on", framesOnLastUpdate, averageFps, lastFrameMs);
	app->win->SetTitle(title);

	// L08: DONE 2: Use SDL_Delay to make sure you get your capped framerate
	if ((cappedMs > 0) && (lastFrameMs < cappedMs))
	{
		// L08: DONE 3: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
		PERF_START(ptimer);
		if(!capped) SDL_Delay(cappedMs - lastFrameMs);
		if(capped) SDL_Delay(cappedMsMin - lastFrameMs);
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) continue;
		
		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) continue;
	
		// L08: DONE 5: Send dt as an argument to all updates, you need
		// to update module parent class and all modules that use update
		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) continue;

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc) return args[index];
	else return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	app->sceneIntro->posContinue = true;
	saveGameRequested = true;
}



// ---------------------------------------
// L02: DONE 5: Create a method to actually load an xml file
// then call all the modules to load themselves
bool App::LoadGame()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(SAVE_STATE_FILENAME);

	if (result != NULL) 
	{
		LOG("Loading new Game State from %s...", SAVE_STATE_FILENAME);

		root = data.child("save_state");

		ListItem<Module*>* item = modules.start;
		ret = true;

		while (item != NULL && ret == true) 
		{
			ret = item->data->LoadState(root.child(item->data->name.GetString()));
			item = item->next;
		}

		//data.reset();
		if (ret == true) LOG("...finished loading");

		else LOG("...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : 0);
	}

	else LOG("Could not parse game state xml file %s. pugi error: %s", loadedGame.GetString(), result.description());

	loadGameRequested = false;

	return ret;
}

// L02: DONE 7: Implement the xml save method for current state
bool App::SaveGame() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", SAVE_STATE_FILENAME);

	//xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("save_state");
	ListItem<Module*>* item = modules.start;
	while (item != NULL && ret == true)
	{
		ret = item->data->SaveState(root.append_child(item->data->name.GetString()));
		item = item->next;
	}
	if (ret == true) 
	{
		data.save_file(SAVE_STATE_FILENAME);
		LOG("... finished saving", );
	}
	else LOG("Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : 0);

	//data.reset();

	saveGameRequested = false;

	return ret;
}



