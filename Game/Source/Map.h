#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "PQueue.h"
#include "Point.h"
#include "DynArray.h"

#include "PugiXml\src\pugixml.hpp"

#define COST_MAP_SIZE	100

// L03: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;

	SDL_Texture* texture;
	int	texWidth;
	int	texHeight;
	int	numTilesWidth;
	int	numTilesHeight;
	int	offsetX;
	int	offsetY;

	// L04: DONE 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;
};

// L03: DONE 1: We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// L06: DONE 5: Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		SString name;
		int value;

	};

	~Properties()
	{
		list.Clear();
	}

	// L06: DONE 7: Method to ask for the value of a custom property
	int GetProperty(const char* name, int default_value = 0) const;

	List<Property*> list;
};

// L04: DONE 1: Create a struct for the map layer
struct MapLayer
{
	SString	name;
	int width;
	int height;
	uint* data;

	// L06: DONE 1: Support custom properties
	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L04: DONE 6: Short function to get the value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

// L03: DONE 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	SDL_Color backgroundColor;
	MapTypes type;
	List<TileSet*> tilesets;

	// L04: DONE 2: Add a list/array of layers to the map
	List<MapLayer*> layers;
};

class Map : public Module
{
public:

	Map();

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();
	void DrawKey();
	void DrawCheckpoint();
	void DrawPuzzle();
	void DrawChest();
	void DrawHeart();
	void DrawColliders();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	// L05: DONE 2: Add orthographic world to map coordinates
	iPoint WorldToMap(int x, int y) const;

	// L12b: Create walkability map for pathfinding
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	// L03: Methods to load all required map data
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);

	// L06: DONE 6: Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	// L06: DONE 3: Pick the right Tileset based on a tile id
	TileSet* GetTilesetFromTileId(int id) const;

public:

	// L03: DONE 1: Add your struct for map info
	MapData data;
	bool colliders = false;
	bool keyTaken = false;
	bool checkpointTaken = false;
	bool puzzleTaken = false;
	bool heartTaken = false;
	bool chestTaken = false;
private:

	pugi::xml_document mapFile;
	SString folder;
	bool mapLoaded;
};

#endif // __MAP_H__