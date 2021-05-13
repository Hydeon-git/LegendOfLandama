
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
		if (layer->data->name == "floor" || layer->data->name == "walls" || layer->data->name == "furniture" || layer->data->name == "extra_furniture" || layer->data->name == "windows" || layer->data->name == "trees" || layer->data->name == "dungeon_walls" || layer->data->name == "dungeon_floor")
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
			if (layer->data->name == "colliders")
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



//
//void Map::DrawVillage()
//{
//	if (mapLoaded == false) return;
//
//	ListItem<MapLayer*>* layer = data.layers.start;
//
//	while (layer != NULL)
//	{
//		if (layer->data->name == "cases" || layer->data->name == "furniture" || layer->data->name == "curtain" || layer->data->name == "windows")
//		{
//			for (int y = 0; y < data.height; ++y)
//			{
//				for (int x = 0; x < data.width; ++x)
//				{
//					int tileId = layer->data->Get(x, y);
//					if (tileId > 0)
//					{
//						TileSet* set = GetTilesetFromTileId(tileId);
//						SDL_Rect rect = set->GetTileRect(tileId);
//						iPoint pos = MapToWorld(x, y);
//						app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
//					}
//				}
//			}
//		}
//		layer = layer->next;
//	}
//}

void Map::DrawDoor()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layerDoorOpen = data.layers.start;
	ListItem<MapLayer*>* layerDoorClosed = data.layers.start;

	while (layerDoorOpen != NULL)
	{

		if (layerDoorOpen->data->name == "door_open")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerDoorOpen->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (puzzle1DungeonDone) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerDoorOpen = layerDoorOpen->next;
	}
	while (layerDoorClosed != NULL)
	{

		if (layerDoorClosed->data->name == "door_close")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerDoorClosed->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!puzzle1DungeonDone) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerDoorClosed = layerDoorClosed->next;
	}
}

void Map::DrawDoorMid()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layerDoorMidOpen = data.layers.start;
	ListItem<MapLayer*>* layerDoorMidClosed = data.layers.start;

	while (layerDoorMidOpen != NULL)
	{

		if (layerDoorMidOpen->data->name == "door_mid_open")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerDoorMidOpen->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (laverFinalPressed) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerDoorMidOpen = layerDoorMidOpen->next;
	}
	while (layerDoorMidClosed != NULL)
	{

		if (layerDoorMidClosed->data->name == "door_mid_close")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerDoorMidClosed->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!laverFinalPressed) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerDoorMidClosed = layerDoorMidClosed->next;
	}
}


void Map::DrawWalls2Dungeon()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	ListItem<MapLayer*>* walls2 = data.layers.start;
	// L06: DONE 4: Make sure we draw all the layers and not just the first one	
	while (walls2 != NULL)
	{
		if (walls2->data->name == "dungeon_walls2")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = walls2->data->Get(x, y);
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
		walls2 = walls2->next;
	}
}

void Map::DrawFloor2Dungeon()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	ListItem<MapLayer*>* floor2 = data.layers.start;
	// L06: DONE 4: Make sure we draw all the layers and not just the first one	
	while (floor2 != NULL)
	{
		if (floor2->data->name == "dungeon_floor2")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = floor2->data->Get(x, y);
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
		floor2 = floor2->next;
	}
}


void Map::DrawChest()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* layerChestOpen = data.layers.start;
	ListItem<MapLayer*>* layerChestClosed = data.layers.start;

	while (layerChestOpen != NULL)
	{

		if (layerChestOpen->data->name == "chest_open")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerChestOpen->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (chestOpened) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerChestOpen = layerChestOpen->next;
	}
	while (layerChestClosed != NULL)
	{

		if (layerChestClosed->data->name == "chest_closed")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = layerChestClosed->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!chestOpened) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		layerChestClosed = layerChestClosed->next;
	}
}


void Map::DrawLavers()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* laver1Open = data.layers.start;
	ListItem<MapLayer*>* laver1Closed = data.layers.start;

	ListItem<MapLayer*>* laver2Open = data.layers.start;
	ListItem<MapLayer*>* laver2Closed = data.layers.start;

	ListItem<MapLayer*>* laver3Open = data.layers.start;
	ListItem<MapLayer*>* laver3Closed = data.layers.start;

	ListItem<MapLayer*>* laverFinalOpen = data.layers.start;
	ListItem<MapLayer*>* laverFinalClosed = data.layers.start;

	//laver1
	while (laver1Open != NULL)
	{

		if (laver1Open->data->name == "lever1_open")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = laver1Open->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (laver1Pressed) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		laver1Open = laver1Open->next;
	}
	while (laver1Closed != NULL)
	{

		if (laver1Closed->data->name == "lever1_close")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = laver1Closed->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!laver1Pressed) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		laver1Closed = laver1Closed->next;
	}

	//laver2
	while (laver2Open != NULL)
	{

		if (laver2Open->data->name == "lever2_open")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = laver2Open->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (laver2Pressed) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		laver2Open = laver2Open->next;
	}
	while (laver2Closed != NULL)
	{

		if (laver2Closed->data->name == "lever2_close")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = laver2Closed->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!laver2Pressed) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		laver2Closed = laver2Closed->next;
	}


	//laver3
	while (laver3Open != NULL)
	{

		if (laver3Open->data->name == "lever3_open")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = laver3Open->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (laver3Pressed) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		laver3Open = laver3Open->next;
	}
	while (laver3Closed != NULL)
	{

		if (laver3Closed->data->name == "lever3_close")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = laver3Closed->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!laver3Pressed) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		laver3Closed = laver3Closed->next;
	}



	//laver Final
	while (laverFinalOpen != NULL)
	{

		if (laverFinalOpen->data->name == "leverFinal_open")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = laverFinalOpen->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (laverFinalPressed) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		laverFinalOpen = laverFinalOpen->next;
	}
	while (laverFinalClosed != NULL)
	{

		if (laverFinalClosed->data->name == "leverFinal_close")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = laverFinalClosed->data->Get(x, y);
					if (tileId > 0)
					{
						TileSet* set = GetTilesetFromTileId(tileId);
						SDL_Rect rect = set->GetTileRect(tileId);
						iPoint pos = MapToWorld(x, y);
						if (!laverFinalPressed) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
					}
				}
			}
		}
		laverFinalClosed = laverFinalClosed->next;
	}



}


void Map::DrawSpikes1()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* spikes1 = data.layers.start;
	//spikes1
	while (spikes1 != NULL)
	{
		if (spikes1->data->name == "spikes_up1")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = spikes1->data->Get(x, y);
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
		spikes1 = spikes1->next;
	}
}

void Map::DrawSpikes2()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* spikes2 = data.layers.start;
	//spikes2
	while (spikes2 != NULL)
	{
		if (spikes2->data->name == "spikes_up2")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = spikes2->data->Get(x, y);
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
		spikes2 = spikes2->next;
	}
}

void Map::DrawSpikes3()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* spikes3 = data.layers.start;
	//spikes3
	while (spikes3 != NULL)
	{
		if (spikes3->data->name == "spikes_up3")
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tileId = spikes3->data->Get(x, y);
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
		spikes3 = spikes3->next;
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

//void Map::DrawChest()
//{
//	if (mapLoaded == false) return;
//
//	ListItem<MapLayer*>* layerHeart = data.layers.start;
//
//	while (layerHeart != NULL)
//	{
//
//		if (layerHeart->data->name == "chest")
//		{
//			for (int y = 0; y < data.height; ++y)
//			{
//				for (int x = 0; x < data.width; ++x)
//				{
//					int tileId = layerHeart->data->Get(x, y);
//					if (tileId > 0)
//					{
//						TileSet* set = GetTilesetFromTileId(tileId);
//						SDL_Rect rect = set->GetTileRect(tileId);
//						iPoint pos = MapToWorld(x, y);
//						if (!heartTaken) app->render->DrawTexture(set->texture, pos.x, pos.y, &rect);
//					}
//				}
//			}
//		}
//		layerHeart = layerHeart->next;
//	}
//}

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