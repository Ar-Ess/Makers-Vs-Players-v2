#ifndef __GROUNDTILE_H__
#define __GROUNDTILE_H__

#include "Tile.h"
#include "Point.h"
#include "External/SDL/include/SDL.h"

struct SDL_Texture;
struct SDL_Rect;

class GroundTile : public Tile
{
public: //Functions

	GroundTile();

	GroundTile(iPoint pos, iPoint coords);

	~GroundTile();

	void Draw();

	iPoint GetCoords() const
	{
		return coordinates;
	}

	SDL_Rect GetRect() const
	{
		return body->ReturnBodyRect();
	}

private:

	SDL_Rect groundSprite[15] = { { 0, 0, 41, 41 }, { 41, 0, 41, 41 }, { 82, 0, 41, 41 }, 
		{ 123, 0, 41, 41 }, { 164, 0, 41, 41 }, { 205, 0, 41, 41 }, { 246, 0, 41, 41 }, 
		{ 287, 0, 41, 41 }, { 328, 0, 41, 41 }, { 369, 0, 41, 41 }, { 410, 0, 41, 41 }, 
		{ 451, 0, 41, 41 }, { 492, 0, 41, 41 }, { 533, 0, 41, 41 }, { 574, 0, 41, 41 }};


	StaticBody* body = nullptr;

};

#endif // __GROUNDTILE_H__
