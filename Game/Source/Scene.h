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

enum Scenes
{
	NO_BODY = -1,
	LOGO_SCENE,
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
	Player* player1 = nullptr;
	Physics* physics = nullptr;

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

	//Updaters
	void UpdateLogoScene(float dt);

private: //BUTTONS
	bool OnGuiMouseClickEvent(GuiControl* control);

	//TEST, for deleting
	DynamicBody* test = nullptr;
};

#endif // __SCENE_H__