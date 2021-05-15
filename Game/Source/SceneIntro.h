#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class GuiControl;
class GuiButton;
class GuiSlider;
class GuiCheckBox;

class SceneIntro : public Module
{
public:
	SceneIntro();
	~SceneIntro();

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

	void Select();

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool CleanUp() override;

	bool posContinue = false;
	bool exit = false;
	bool startClicked = false;

	bool guiColliders = false;

	bool options = false;

private:

	int posLogoRight = 1150;

	bool active = true;
	SString name;
	GuiButton* btnStart;
	GuiButton* btnContinue;
	GuiButton* btnOptions;
	GuiButton* btnExit;
	GuiButton* btnBackOptions;

	GuiSlider* sliderMusicVolume;
	GuiSlider* sliderFxVolume;

	GuiCheckBox* checkBoxFullscreen;
	GuiCheckBox* checkBoxVSync;

	SDL_Texture* introText = nullptr;
	SDL_Texture* selectorTex = nullptr;

	SDL_Texture* logoBackText = nullptr;
	SDL_Texture* logoRightText = nullptr;
	SDL_Texture* logoMidText = nullptr;
	SDL_Texture * logoLeftText = nullptr;

	SDL_Rect* fullscreenRect;

	uint logoFx = 0;

	int whiteFont = -1;
	int pos = 0;
	int posScaleY = 0, posScaleX = 0;
	int count = 0;
};


#endif //__SCENE_INTRO_H__