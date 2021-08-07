#ifndef __TILE_H__
#define __TILE_H__

#include "Point.h"
#include "External/SDL/include/SDL.h"
#include "Physics.h"

struct SDL_Texture;
struct SDL_Rect;

class Tile
{
public:

	Tile() {}

	virtual ~Tile() {}

	Tile(iPoint pos, iPoint coords) {}

	virtual void Draw() {}

	virtual iPoint GetCoords() const
	{
		return coordinates;
	}

public:

	SDL_Texture* texture = nullptr;

	iPoint position;

	iPoint coordinates;


};

#endif // __TILE_H__
