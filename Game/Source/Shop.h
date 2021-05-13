#ifndef _SHOP_H_
#define _SHOP_H_

#include "Module.h"
#include "Animation.h"


struct SDL_Texture;

struct ItemsPrice {
	bool selected;
	int priceItem;
};

class Shop : public Module
{
public:
	//Constructor
	Shop();

	//Destructor
	~Shop();

	bool Awake(pugi::xml_node& node);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	// Sound effects indices
	uint chooseFx = 0;
	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background

	void select();

	void activeSelected(int _items);


public:
	enum SHOP_SLOT : int {
		
		WOOD_SWORD, 
		STEEL_SWORD,
		GOLD_SWORD,
		FIRE_SWORD,
		WOOD_PROTECTION,
		STEEL_PROTECTION,
		GOLD_PROTECTION,
		FIRE_PROTECTION,
		WOOD_BOW,
		STEEL_BOW,
		GOLD_BOW,
		DIAMOND_BOW,
		WOOD_WAND,
		AMATIST_WAND,
		RUBY_WAND,
		DIAMOND_WAND,
		EXIT,

	};
	
	void getOwnedItems();

	SDL_Texture* shopTex = nullptr;
	SDL_Texture* selectorTex = nullptr;
	SDL_Texture* itemSold = nullptr;

	int shopX = 0, shopY = 0;
	int itemPos = 0;
	uint OptionSelection = 0;
	uint SelectItem = 0;
	uint InsuficientMoney = 0;

	bool keyDownPad = false;
	int shopFont = 0;

	int selectedItem;
	bool selectorActive;

	bool staticPlayer;
	bool begin = false;
	bool shoploop;

	bool infiniteMoney = false;

	bool open = false;

	ItemsPrice items[15];

	char moneyText[10] = { "\0" };






};
#endif // !_Shop_H_