#ifndef _HUD_H_
#define _HUD_H_

#include "Module.h"
#include "Animation.h"


struct SDL_Texture;


class HUD : public Module
{
public:
	//Constructor
	HUD();

	//Destructor
	~HUD();

	bool Awake(pugi::xml_node& node);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void openBackPack();
	// Sound effects indices
	uint chooseFx = 0;
	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background

	bool bagIsOpen;

	int coins;

private:
	

	SDL_Texture* bagTex = nullptr;
	SDL_Texture* coinTex = nullptr;

	SDL_Texture* openbagTex = nullptr;

	void drawCoins();




};
#endif // !_HUD_H_