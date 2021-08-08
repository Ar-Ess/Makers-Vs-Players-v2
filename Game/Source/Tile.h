#ifndef __TILE_H__
#define __TILE_H__

#include "Point.h"
#include "External/SDL/include/SDL.h"
#include "Physics.h"
#include "Textures.h"

struct SDL_Texture;
class LevelEditor;
struct SDL_Rect;

enum class TileType
{
	NO_TILE = 0,
	GROUND
};

class Tile
{
public:

	Tile() {}

	virtual ~Tile() 
	{
		app->tex->UnLoad(texture);
	}

	Tile(iPoint pos, iPoint coords) {}

	virtual void Draw() {}

	virtual iPoint GetCoords() const
	{
		return coordinates;
	}

public:

	SDL_Texture* texture = nullptr;
	LevelEditor* editor = nullptr;
	TileType type = TileType::NO_TILE;
	fPoint position;
	iPoint coordinates;

};

#endif // __TILE_H__
