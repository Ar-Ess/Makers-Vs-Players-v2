#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Point.h"
#include "DynArray.h"

#include "Spline.h"
#include "Physics.h"
#include "EasingFunctions.h"

struct SDL_Texture;
class Map;
class Player;
class Physics;
class LevelEditor;

enum Scenes
{
	NO_BODY = -1,
	LOGO_SCENE,
	MAIN_MENU,
	LEVEL_EDITOR
};

class Scene : public Module
{
public:

	Scene();

	virtual ~Scene();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	Scenes GetCurrScene() const
	{
		return currScene;
	}

private: //DEBUG

	void DebugCommands();
	bool exit = false;
	bool activeContinue = false;

private: //Pointers
	friend class Map;
	LevelEditor* levelEditor = nullptr;

private: //SPLINES
	Spline spline;
	EasingFunctions easing;

public://Scene Manager
	void SetScene(Scenes scene);

private:
	Scenes currScene = NO_BODY;
	Scenes prevScene = NO_BODY;

	//Setters
	void SetLogoScene();
	void SetMainMenu();
	void SetLevelEditor();

	//Updaters
	void UpdateLogoScene(float dt);
	void UpdateMainMenu(float dt);
	void UpdateLevelEditor(float dt);

private: //BUTTONS
	bool OnGuiMouseClickEvent(GuiControl* control);
};

#endif // __SCENE_H__