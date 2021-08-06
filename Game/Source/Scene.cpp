#define _CRT_SECURE_NO_WARNINGS

#include "App.h"
#include "Scene.h"

#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Physics.h"
#include "LevelEditorScene.h"

#include "GuiManager.h"
#include "GuiString.h"
#include "GuiControl.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "FontTTF.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

Scene::Scene() : Module()
{

}

Scene::~Scene()
{}

bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	return ret;
}

bool Scene::Start()
{
	player1 = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	physics = new Physics();

	//DEBUG BOOLS
	app->guiManager->debugGui = false;

	//FIRST SCENE
	SetScene(LOGO_SCENE);

	//CONTINUE ACTIVITY
	activeContinue = false;
	if (FILE* file = fopen("save_game.xml", "r"))
	{
		fclose(file);
		activeContinue = true;
	}

	//SPLINE
	pugi::xml_document doc;
	spline.LoadSplines(doc);

	app->render->scale = 1; //Qui toqui aquesta linia de codi, la 72, i m'entero, no viu un dia més :) <3

	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update(float dt)
{
	if (currScene == LOGO_SCENE) UpdateLogoScene(dt);
	else if (currScene == MAIN_MENU) UpdateMainMenu(dt);
	else if (currScene == LEVEL_EDITOR) UpdateLevelEditor(dt);
	
	return true;
}

bool Scene::PostUpdate()
{
	DebugCommands();

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		app->guiManager->debugGui = !app->guiManager->debugGui;
	}

	return !exit;
}

bool Scene::CleanUp()
{
	LOG("Freeing scene");

	if (currScene == LOGO_SCENE)
	{
	}
	else if (currScene == MAIN_MENU)
	{
	}
	else if (currScene == LEVEL_EDITOR)
	{
	}

	return true;
}

// SCENE MANAGER

void Scene::SetScene(Scenes scene)
{
	CleanUp();

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();
	else if (scene == MAIN_MENU) SetMainMenu();
	else if (scene == LEVEL_EDITOR) SetLevelEditor();

	easing.ResetIterations();
}

void Scene::SetLogoScene()
{

}

void Scene::SetMainMenu()
{

}

void Scene::SetLevelEditor()
{
	levelEditor = new LevelEditor();
	player1->body = (DynamicBody*)physics->CreateBody(BodyType::DYNAMIC_BODY, fPoint{ 300.0f, 700.0f }, { 300, 700, 20, 40 }, { 0, 0 }, {0, 0}, 1.5f);
	physics->SetPhysicsPreset(PhysicsPreset::PLATFORMER_PHYSICS_PRESET);
	physics->PausePhysics(true);
	//physics->SetScenarioPreset(ScenarioPreset::PLATFORMER_1280x720_SCENARIO_PRESET);

	levelEditor->Start();
}

void Scene::UpdateLogoScene(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) SetScene(MAIN_MENU);
}

void Scene::UpdateMainMenu(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) SetScene(LEVEL_EDITOR);
}

void Scene::UpdateLevelEditor(float dt)
{
	levelEditor->Update(dt, player1, physics);
	levelEditor->Draw(player1, physics);
}

// GUI CONTROLS

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (currScene)
	{
	case LOGO_SCENE:
		break;
	case MAIN_MENU:
		break;
	case LEVEL_EDITOR:
		break;
	}

	return true;
}

void Scene::DebugCommands()
{
	switch (currScene)
	{
	case LOGO_SCENE:
		break;
	case MAIN_MENU:
		break;
	case LEVEL_EDITOR:
		break;
	}
}
