#ifndef __TILE_H__
#define __TILE_H__

#include "Point.h"
#include "External/SDL/include/SDL.h"
#include "Physics.h"
#include "Textures.h"
#include "Utils.h"

struct SDL_Texture;
class LevelEditor;
struct SDL_Rect;

enum class TileType
{
	NO_TILE = 0,
	GROUND,
	COIN,
	BRICK,
	SPIKE_TRAP,
	ARROW_SIGN
};

enum class Z
{
	BACK,
	MID,
	FRONT
};

class Tile
{
public:

	Tile() {}

	Tile(fPoint pos, iPoint coords) {}

	virtual ~Tile() 
	{
		app->tex->UnLoad(texture);
		texture = nullptr;
	}

	virtual void Update(float dt) {}

	virtual void Draw(float dt) {}

	virtual void Restart() {}

	virtual iPoint GetCoords() const
	{
		return coordinates;
	}

	virtual Body* GetBody() { return nullptr; };

public:

	SDL_Texture* texture = nullptr;
	LevelEditor* editor = nullptr;
	TileType type = TileType::NO_TILE;
	Z zAxis;
	fPoint position;
	iPoint coordinates;
	Utils utils;
	bool fourByFour = false;

};

#endif // __TILE_H__
