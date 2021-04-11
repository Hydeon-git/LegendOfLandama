
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name.Create("map");
}

// Destructor
Map::~Map()
{
}

// L06: DONE 7: Ask for the value of a custom property
int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value) return item->data->value;
		item = item->next;
	}

	return defaultValue;
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.Create(config.child("folder").child_value());

	return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	ListItem<MapLayer*>* layer = data.layers.start;
	// L06: DONE 4: Make sure we draw all the layers and not just the first one	
	while (layer != NULL)
	{
		if (layer->data->name == "ground")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layer->data->Get(x, y);
					if (tileId > 0)
					{
						// L04: DONE 9: Complete the draw function
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layer = layer->next;
	}
}

void Map::DrawColliders()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layer = data.layers.start;
	while (layer != NULL)
	{
		if (colliders)
		{
			if (layer->data->name == "collisions")
			{
				for (int y = 0; y < data.height; ++y)
				{
					for (int x = 0; x < data.width; ++x)
					{
						int tileId = layer->data->Get(x, y);
						if (tileId > 0)
						{
							TileSet* set = GetTilesetFromTileId(tileId);
							SDL_Rect rect = set->GetTileRect(tileId);
							iPoint pos = MapToWorld(x, y);
							app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
						}
					}
				}
			}
		}
		layer = layer->next;
	}
}

void Map::DrawKey()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layerNoKey = data.layers.start;
	ListItem<MapLayer*>* layerKey = data.layers.start;

	while (layerNoKey != NULL)
	{

		if (layerNoKey->data->name == "keyTaken")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerNoKey->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (keyTaken) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerNoKey = layerNoKey->next;
	}
	while (layerKey != NULL)
	{

		if (layerKey->data->name == "key")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerKey->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!keyTaken) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerKey = layerKey->next;
	}
}

void Map::DrawCheckpoint()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layerCheckpointTaken = data.layers.start;
	ListItem<MapLayer*>* layerCheckpoint = data.layers.start;

	while (layerCheckpointTaken != NULL)
	{

		if (layerCheckpointTaken->data->name == "checkpointTaken")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerCheckpointTaken->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (checkpointTaken) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerCheckpointTaken = layerCheckpointTaken->next;
	}
	while (layerCheckpoint != NULL)
	{

		if (layerCheckpoint->data->name == "checkpoint")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerCheckpoint->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!checkpointTaken) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerCheckpoint = layerCheckpoint->next;
	}
}

void Map::DrawPuzzle()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layerPuzzleTaken = data.layers.start;
	ListItem<MapLayer*>* layerPuzzle = data.layers.start;

	while (layerPuzzleTaken != NULL)
	{

		if (layerPuzzleTaken->data->name == "puzzleTaken")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerPuzzleTaken->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (puzzleTaken) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerPuzzleTaken = layerPuzzleTaken->next;
	}
	while (layerPuzzle != NULL)
	{

		if (layerPuzzle->data->name == "puzzle")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerPuzzle->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!puzzleTaken) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerPuzzle = layerPuzzle->next;
	}
}


void Map::DrawHeart()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layerHeart = data.layers.start;

	while (layerHeart != NULL)
	{

		if (layerHeart->data->name == "heart")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerHeart->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!heartTaken) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerHeart = layerHeart->next;
	}
}

void Map::DrawChest()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layerHeart = data.layers.start;

	while (layerHeart != NULL)
	{

		if (layerHeart->data->name == "chest")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerHeart->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!heartTaken) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerHeart = layerHeart->next;
	}
}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	// L05: DONE 1: Add isometric map to world coordinates
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth / 2);
		ret.y = (x + y) * (data.tileHeight / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// L05: DONE 2: Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	// L05: DONE 3: Add the case for isometric maps to WorldToMap
	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		float halfWidth = data.tileWidth * 0.5f;
		float halfHeight = data.tileHeight * 0.5f;
		ret.x = int((x / halfWidth + y / halfHeight) / 2);
		ret.y = int((y / halfHeight - (x / halfWidth)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

// L06: DONE 3: Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// L04: DONE 7: Get relative Tile rectangle
	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));

	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.Clear();

	// L04: DONE 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.Clear();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	// Load general info
	if (ret == true) ret = LoadMap();

	// L03: DONE 4: Create and call a private function to load a tileset
	// remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);
		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.Add(set);
	}

	// L04: DONE 4: Iterate all layers and load each of them
	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true) data.layers.Add(lay);
	}

	if (ret == true)
	{
		// L03: T0D0 5: LOG all the data loaded iterate all tilesets and LOG everything
		// L04: T0D0 4: LOG the info for each loaded layer
	}

	mapLoaded = ret;

	return ret;
}

// L03: DONE: Load map general properties
bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE: Load map general properties
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		SString bg_color(map.attribute("backgroundcolor").as_string());

		data.backgroundColor.r = 0;
		data.backgroundColor.g = 0;
		data.backgroundColor.b = 0;
		data.backgroundColor.a = 0;

		if (bg_color.Length() > 0)
		{
			SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.backgroundColor.b = v;
		}

		SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal") data.type = MAPTYPE_ORTHOGONAL;
		else if (orientation == "isometric") data.type = MAPTYPE_ISOMETRIC;
		else if (orientation == "staggered") data.type = MAPTYPE_STAGGERED;
		else data.type = MAPTYPE_UNKNOWN;
	}

	return ret;
}

// L03: DONE: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// L03: DONE: Load Tileset attributes
	set->name.Create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offsetX = offset.attribute("x").as_int();
		set->offsetY = offset.attribute("y").as_int();
	}
	else
	{
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}

// L03: DONE: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE: Load Tileset image
		set->texture = app->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->texWidth = image.attribute("width").as_int();

		if (set->texWidth <= 0)	set->texWidth = w;

		set->texHeight = image.attribute("height").as_int();

		if (set->texHeight <= 0) set->texHeight = h;

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
	}

	return ret;
}

// L04: DONE 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	// L04: DONE 3: Load a single layer
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	pugi::xml_node layer_data = node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width * layer->height];
		memset(layer->data, 0, layer->width * layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

// L06: DONE 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;
	Properties::Property* property = new Properties::Property;
	pugi::xml_node pnode = node;
	for (pnode = node.child("properties").child("property"); pnode; pnode = pnode.next_sibling("property"))
	{
		property->name = pnode.attribute("name").as_string();
		property->value = pnode.attribute("value").as_int();
		properties.list.Add(property);
	}
	return ret;
}

// L12b: Create walkability map for pathfinding
bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item;
	item = data.layers.start;

	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.GetProperty("Navigation", 0) == 1) continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y * layer->width) + x;

				int tileId = layer->Get(x, y);
				TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

				if (tileset != NULL) map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}