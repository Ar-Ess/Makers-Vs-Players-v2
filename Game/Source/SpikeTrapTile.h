#ifndef __SPIKETRAPTILE_H__
#define __SPIKETRAPTILE_H__

#include "Tile.h"
#include "Point.h"
#include "External/SDL/include/SDL.h"
//#include "LevelEditorScene.h"

struct SDL_Texture;
struct SDL_Rect;
//class LevelEditor;

class SpikeTrapTile : public Tile
{
public: //Functions

	SpikeTrapTile();

	SpikeTrapTile(fPoint pos, iPoint coords, LevelEditor* lE, Z axis);

	~SpikeTrapTile();

	void Update(float dt);

	void Draw(float dt);

	void Restart();

	friend void LevelEditor::PreviewDeath();

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

	StaticBody* body = nullptr;
};

#endif // __SPIKETRAPTILE_H__

