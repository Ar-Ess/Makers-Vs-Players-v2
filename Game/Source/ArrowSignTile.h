#ifndef __ARROWSIGNTILE_H__
#define __ARROWSIGNTILE_H__

#include "Tile.h"
#include "Point.h"
#include "External/SDL/include/SDL.h"

struct SDL_Texture;
struct SDL_Rect;

class ArrowSignTile : public Tile
{
public: //Functions

	ArrowSignTile();

	ArrowSignTile(fPoint pos, iPoint coords, LevelEditor* lE, Z axis, int angl);

	~ArrowSignTile();

	void Start();

	void Update(float dt);

	void Draw(float dt);

	void Restart();

	iPoint GetCoords() const
	{
		return coordinates;
	}

	Body* GetBody()
	{
		return (Body*)body;
	}

private:

	SDL_Texture* texture = nullptr;
	int angle = 0;
	StaticBody* body = nullptr;
	SDL_Rect rect = {};
};

#endif // __ARROWSIGNTILE_H__
