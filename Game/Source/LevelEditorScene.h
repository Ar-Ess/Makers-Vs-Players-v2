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

#define MAX_BACKGROUND 2

#include "List.h"
#include "Point.h"
#include "Utils.h"
#include "Tile.h"

class Player;
class Physics;
class Tile;
struct SDL_Texture;

enum class EditorState
{
	EDITING = 0,
	PLAYING,
	PAUSE_MENU
};

enum class Selection
{
	NO_SELECT = 0,
	ERASE,
	GROUND,
	COIN,
	BRICK,
	SPIKE_TRAP,
	ARROW_SIGN
};

enum class Background
{
	SKY = 1,
	NOON
};

class LevelEditor
{
public:

	LevelEditor();

	~LevelEditor();

	void Start();

	void Update(float dt);

	void Draw(float dt);

	void DrawBackground();

	void DebugDraw();

	void CleanUp();

public:

	void LoadBackgroundImages();
	void ChangeEditorState(EditorState state);
	Background GetBackground() const
	{
		return bg;
	}
	EditorState GetState() const
	{
		return state;
	}

private:
	friend class SpikeTrapTile;

	//EDITOR FUNCTIONS
	void UpdateEditor(float dt);
	void TileSelectionLogic();
	void CameraDisplace();
	void ScreenAddition();
	void ScreenRemoving(int screen);
	void TileMenuLogic();
	void TilePlaceLogic();
	void TileRemoveLogic();
	void RotationLogic();
	iPoint GetCoordsFromMousePos();
	Tile* GetTileFromCoords(iPoint coords);
	// This function only works between 1x1 tile checking
	bool TileExistance(iPoint coords);
	// This function works for all type of tiles (1x1, 2x2...) but asking for the selection (specially for TilePlaceLogic() function)
	bool TileExistance(iPoint coords, Selection select);
	// This function works for all type of tiles (1x1, 2x2...) but asking for the type of the tile (specially for TileRemoveLogic() function)
	bool TileExistance(iPoint coords, TileType type);
	void DeleteTileLogic(iPoint coords);
	void PlayerDragLogic();
	bool IsMouseInPlayer();
	iPoint GetMousePosInPlayer(iPoint pos);
	void ChangeTilesetLogic();

	//PREVIEW FUNCTIONS
	void UpdatePreview(float dt);
	void OutBoundsDeath();
	void PreviewDeath();

public:
	Physics* phys = nullptr;
	Player* player = nullptr;
	List<Tile*> tiles;

private:

	int currScreen = 0;
	const int wTileScreen[11] = {0, 24, 48, 72, 96, 120, 144, 168, 192, 216, 240};
	const int maxScreens = 10;
	int arrowAngle = 0;
	bool menu = false;
	iPoint deltaPosition = {};

	Selection select = Selection::NO_SELECT;
	EditorState state = EditorState::EDITING;
	Background bg = Background::SKY;
	Utils utils;

	SDL_Texture* background[4] = {nullptr, nullptr, nullptr, nullptr};
};

#endif // __LEVEL_EDITOR_H__
