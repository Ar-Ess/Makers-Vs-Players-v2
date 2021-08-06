#ifndef __LEVEL_EDITOR_H__
#define __LEVEL_EDITOR_H__

#define TILE_SIZE 53
#define INIT_CAM_X 0
#define INIT_CAM_Y 711
#define MAX_SCREEN_X 12720
#define MAX_SCREEN_Y 1431
#define CAM_VEL 6

class Player;
class Physics;

enum class EditorState
{
	EDITING = 0,
	PLAYING,
	PAUSE_MENU
};

class LevelEditor
{
public:

	LevelEditor();

	~LevelEditor();

	void Start();

	void Update(float dt, Player* p, Physics* phys);

	void Draw(Player* p, Physics* phys);

	void DebugDraw();

	void CleanUp();

private:

	//EDITOR FUNCTIONS
	void UpdateEditor(Player* p);
	void CameraDisplace();

	//PREVIEW FUNCTIONS
	void UpdatePreview(Player* p);

private:
	int currScreen = 0;
	const int wTileScreen[10] = {24, 48, 72, 96, 120, 144, 168, 192, 216, 240};
	const int hTileScreen = 27;
	const int maxScreens = 10;

	EditorState state = EditorState::EDITING;
};

#endif // __LEVEL_EDITOR_H__
