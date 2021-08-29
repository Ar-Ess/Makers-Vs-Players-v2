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

	GroundTile(fPoint pos, iPoint coords, LevelEditor* lE, Z axis);

	~GroundTile();

	void Update(float dt);

	void Draw(float dt);

	void Restart();

	iPoint GetCoords() const
	{
		return coordinates;
	}

	SDL_Rect GetRect() const
	{
		return body->ReturnBodyRect();
	}

	Body* GetBody()
	{
		return (Body*)body;
	}

private:

	SDL_Rect sprites[15] = { { 0, 0, 53, 53 }, { 53, 0, 53, 53 }, { 106, 0, 53, 53 },
		{ 159, 0, 53, 53 }, { 212, 0, 53, 53 }, { 265, 0, 53, 53 }, { 318, 0, 53, 53 },
		{ 371, 0, 53, 53 }, { 424, 0, 53, 53 }, { 477, 0, 53, 53 }, { 530, 0, 53, 53 },
		{ 583, 0, 53, 53 }, { 636, 0, 53, 53 }, { 689, 0, 53, 53 }, { 742, 0, 53, 53 }};

	StaticBody* body = nullptr;
};

#endif // __GROUNDTILE_H__
