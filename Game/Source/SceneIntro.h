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

	bool OnGuiMouseClickEvent(GuiControl* control);


	bool CleanUp() override;

	bool posContinue = false;
	bool exit = false;
	bool playClicked = false;

	bool guiColliders = false;

	bool settings = false;
	bool credits = false;
	bool controls = false;

private:
	bool active = true;
	SString name;
	GuiButton* btnPlay;
	GuiButton* btnContinue;
	GuiButton* btnSettings;
	GuiButton* btnCredits;
	GuiButton* btnControls;
	GuiButton* btnBackControls;
	GuiButton* btnExit;
	GuiButton* btnBackSettings;
	GuiButton* btnBackCredits;

	GuiSlider* sliderMusicVolume;
	GuiSlider* sliderFxVolume;

	GuiCheckBox* checkBoxFullscreen;
	GuiCheckBox* checkBoxVSync;

	SDL_Texture* introText = nullptr;
	SDL_Texture* creditText = nullptr;
	SDL_Texture* logoText = nullptr;

	SDL_Rect* fullscreenRect;

	int whiteFont = -1;
	int yellowFont = -1;	

	int count = 0;
};


#endif //__SCENE_INTRO_H__