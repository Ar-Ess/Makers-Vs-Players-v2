#ifndef __BRICKTILE_H__
#define __BRICKTILE_H__

#include "Tile.h"
#include "Point.h"
#include "External/SDL/include/SDL.h"
#include "Coin.h"

struct SDL_Texture;
struct SDL_Rect;

enum class BrickState
{
	BRICK,
	COIN,
	COLLECTED
};

class BrickTile : public Tile
{
public:

	BrickTile();

	BrickTile(fPoint pos, iPoint coords, LevelEditor* lE, Z axis);

	~BrickTile();

	void Start();

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

public:

	Animation coinAnim;
	SDL_Rect rect = {};
	SDL_Rect sprites[4] = { {0, 0, 53, 53}, {53, 0, 53, 53}, {106, 0, 53, 53}, {159, 0, 53, 53} };
	StaticBody* body = nullptr;
	BrickState state = BrickState::BRICK;

	unsigned short int timer = 0;
	bool pSwitch = false;

};

#endif // __BRICKTILE_H__

