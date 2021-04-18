#ifndef __SCENE_LOSE_H__
#define __SCENE_LOSE_H__

#include "Module.h"
#include "Animation.h"

class GuiControl;
class GuiButton;

struct SDL_Texture;

class SceneLose : public Module
{
public:
	SceneLose();
	~SceneLose();

	// Called when the module is activated
	bool Awake(pugi::xml_node& config) override;
	// Loads the necessary textures for the map background
	bool Start();

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate();

	bool OnGuiMouseClickEvent(GuiControl* control);
	bool CleanUp() override;

	bool lost = false;
	bool guiColliders = false;
private:

	GuiButton* btnRestart;
	GuiButton* btnExit;
	int whiteFont = -1;
	SDL_Texture* loseText = nullptr;
	
	bool exit = false;
};



#endif //__SCENE_LOSE_H__