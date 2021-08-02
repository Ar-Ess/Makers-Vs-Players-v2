#include "Map.h"
#include "App.h"
#include "Render.h"
#include "Scene.h"
#include "Collider.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Entity(EntityType::MAP)
{
	mapLoaded = false;
	folder.Create("Assets/Maps");
}

Map::~Map()
{}

int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return defaultValue;
}

bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    return ret;
}

void Map::UpdateLayers()
{
	data.orgLayers.Clear();
	List<int> list;
	for (int i = 0; i < data.layers.Count(); i++) list.Add(data.layers[i]->properties.GetProperty("y"));
	list.BubbleSort();
	for (int i = 0; i < list.Count(); i++) data.orgLayers.PushBack(GetLayerFromY(list[i]));
}

void Map::Draw()
{
	Player* p = app->scene->player1;

	bool playerDraw = false;
	if (mapLoaded == false) return;

	camOffset.x = app->render->camera.x;
	camOffset.y = app->render->camera.y;

	UpdateLayers();

	for (int i = 0; i < data.orgLayers.Count(); i++)
	{
		MapLayer* layer = data.orgLayers[i];
		if (strcmp(layer->name.GetString(), "colliders") != 0)
		{
			if (layer->properties.GetProperty("y") * SCALE < p->rect.y)
			{
				if ((layer->properties.GetProperty("drawable", 1) != 0) || drawColliders) DrawLayer(i);
			}
			else
			{
				if (!playerDraw)
				{
					//DRAW PLAYER
					playerDraw = true;
					i--;
				}
				else
				{
					if ((layer->properties.GetProperty("drawable", 1) != 0) || drawColliders) DrawLayer(i);
				}
			}
		}
		layer = nullptr;
	}
}

void Map::DrawLayer(int num)
{
	if (num < data.layers.Count())
	{
		app->render->scale = scale;

		// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				//CANVIAR
				int tileId = 0;
				tileId = data.orgLayers[num]->Get(x, y);

				//-----

				if (tileId > 0)
				{
					// L04: DONE 9: Complete the draw function
					TileSet* tileset = GetTilesetFromTileId(tileId);

					SDL_Rect rec = GetTilesetFromTileId(tileId)->GetTileRect(tileId);
					iPoint pos = MapToWorld(x, y);

					SDL_Rect invertCam = { -app->render->camera.x, -app->render->camera.y, app->render->camera.w, app->render->camera.h };

					if (colliderUtils.CheckCollision(invertCam, { (int)((pos.x + tileset->offsetX) * 1.75), (int)((pos.y + tileset->offsetY) * 1.75), (int)(16 * 1.75), (int)(16 * 1.75) })) app->render->DrawTexture(tileset->texture, pos.x + tileset->offsetX, pos.y + tileset->offsetY, SCALE, SCALE, &rec);

					tileset = nullptr;
				}
			}
		}
		app->render->scale = 1;
	}
}

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

SDL_Rect Map::GetTilemapRec(int x, int y) const
{
	iPoint pos = MapToWorld(x, y);
	SDL_Rect rec = { pos.x * scale + camOffset.x, pos.y * scale + camOffset.y, 
					 data.tileWidth * scale, data.tileHeight * scale };

	return rec;
}

MapLayer* Map::GetLayerFromY(int y) const
{
	ListItem<MapLayer*>* item = data.layers.start;
	MapLayer* set = item->data;

	while (item)
	{
		if (y == item->data->properties.GetProperty("y"))
		{
			set = item->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

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

bool Map::CleanUp()
{
    LOG("Unloading map");

    // L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		app->tex->UnLoad(item->data->texture);
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

	data.orgLayers.Clear();

	// Clean up the pugui tree
	mapFile.reset();

    return true;
}

bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s/%s", folder.GetString(), filename);

    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

	// Load general info
    if(ret == true)
    {
        // L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

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

		if (strcmp(lay->name.GetString(), "colliders") == 0)
		{
			LoadColliders(lay);
		}
	}
    
    if(ret == true)
    {
        // L03: DONE 5: LOG all the data loaded iterate all tilesets and LOG everything
		//LOG("Successfully parsed map XML file: %s", filename);
		//LOG("width: %d height: %d", data.width, data.height);
		//LOG("tile_width: %d tile_height: %d", data.tileWidth, data.tileHeight);

		ListItem<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			//LOG("Tileset ----");
			//LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			//LOG("tile width: %d tile height: %d", s->tileWidth, s->tileHeight);
			//LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// L04: DONE 4: LOG the info for each loaded layer
		ListItem<MapLayer*>* itemLayer = data.layers.start;
		while (itemLayer != NULL)
		{
			MapLayer* l = itemLayer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			itemLayer = itemLayer->next;
		}
	}

    mapLoaded = ret;

	tmp.Clear();

    return ret;
}

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
		/*
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
		*/

		SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal") data.type = MAPTYPE_ORTHOGONAL;
		else if (orientation == "isometric") data.type = MAPTYPE_ISOMETRIC;
		else if (orientation == "staggered") data.type = MAPTYPE_STAGGERED;
		else data.type = MAPTYPE_UNKNOWN;
	}

	return ret;
}

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
		//set->offsetX = offset.attribute("x").as_int();
		//set->offsetY = offset.attribute("y").as_int();
	}
	else
	{
		set->offsetX = 0;
		set->offsetY = 0;
	}

	return ret;
}

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

		if (set->texWidth <= 0)
		{
			set->texWidth = w;
		}

		set->texHeight = image.attribute("height").as_int();

		if (set->texHeight <= 0)
		{
			set->texHeight = h;
		}

		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
	}

	return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	
	// L04: DONE 3: Load a single layer
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	pugi::xml_node layerData = node.child("data");

	if (layerData == NULL)
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
		for (pugi::xml_node tile = layerData.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	// Load layer properties
	LoadProperties(node, layer->properties);

	return ret;
}	

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.Add(p);
		}
	}
	
	return ret;
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item;
	item = data.layers.start;

	for(item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if(layer->properties.GetProperty("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tileId = layer->Get(x, y);
				TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;
				
				if(tileset != NULL)
				{
					map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
				}
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

void Map::LoadColliders(MapLayer* layer)
{
	for (int y = 0; y < data.height; ++y)
	{
		for (int x = 0; x < data.width; ++x)
		{
			uint tileId = layer->Get(x, y);

			if (tileId > 0)
			{
				TileSet* set = data.tilesets.end->data;

				SDL_Rect r = set->GetTileRect(tileId);

				iPoint pos = MapToWorld(x, y);

				r.x = pos.x * SCALE;
				r.y = pos.y * SCALE;
				r.w *= SCALE;
				r.h *= SCALE;

				//if (app->scene->world->GetPlace() == Places::HOUSE)
				//{
				//	switch (tileId)
				//	{
				//	case 527: // COLLIDER
				//		app->scene->world->collisions.Add(r);
				//		break;

				//	case 526: // VILLAGE
				//		app->scene->world->location1.Add(r);
				//		break;
				//	}
				//}
			}
		}
	}
}