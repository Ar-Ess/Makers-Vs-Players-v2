#ifndef __LEVEL_EDITOR_H__
#define __LEVEL_EDITOR_H__

#define TILE_SIZE 53
#define TILE_PER_SCREEN_W 24
#define TILE_PER_SCREEN_H 27
#define INIT_CAM_X 0
#define INIT_CAM_Y 711
#define MAX_SCREEN_X 12720
#define MAX_SCREEN_Y 1431
#define CAM_VEL 9

#include "List.h"
#include "Point.h"

class Player;
class Physics;
class Tile;

enum class EditorState
{
	EDITING = 0,
	PLAYING,
	PAUSE_MENU
};

enum class TileSelect
{
	NO_SELECT = 0,
	ERASE,
	GROUND
};

class LevelEditor
{
public:

	LevelEditor();

	~LevelEditor();

	void Start();

	void Update(float dt);

	void Draw();

	void DebugDraw();

	void CleanUp();

private:

	//EDITOR FUNCTIONS
	void UpdateEditor();
	void TileSelectionLogic();
	void CameraDisplace();
	void ScreenAddition();
	void TilePlacement();
	void TileRemoveLogic();
	iPoint GetCoordsFromMousePos();
	bool TileExistance(iPoint coords);
	void DeleteTile(iPoint coords);

	//PREVIEW FUNCTIONS
	void UpdatePreview();

public:
	Physics* phys = nullptr;
	Player* player = nullptr;
	List<Tile*> tiles;

private:

	int currScreen = 0;
	const int wTileScreen[11] = {0, 24, 48, 72, 96, 120, 144, 168, 192, 216, 240};
	const int maxScreens = 10;

	TileSelect tSelect = TileSelect::NO_SELECT;

	EditorState state = EditorState::EDITING;
};

#endif // __LEVEL_EDITOR_H__
