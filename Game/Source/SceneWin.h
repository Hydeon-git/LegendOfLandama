#ifndef __SCENE_WIN_H__
#define __SCENE_WIN_H__

#include "Module.h"
#include "Animation.h"

class GuiControl;
class GuiButton;

struct SDL_Texture;

class SceneWin : public Module
{
public:
	SceneWin();
	~SceneWin();

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

	bool won = false;
	bool guiColliders = false;
private:
	GuiButton* btnRestart;
	GuiButton* btnExit;
	int whiteFont = -1;
	SDL_Texture* winText = nullptr;
	bool exit = false;
};
#endif //__SCENE_WIN_H__