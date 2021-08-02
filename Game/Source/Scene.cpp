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
#include "DialogueManager.h"
#include "Transition.h"
#include "Map.h"
#include "Boss.h"
#include "Physics.h"

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

	return true;
}

// SCENE MANAGER

void Scene::SetScene(Scenes scene)
{
	CleanUp();

	prevScene = currScene;
	currScene = scene;

	if (scene == LOGO_SCENE) SetLogoScene();

	easing.ResetIterations();
}

void Scene::SetLogoScene()
{	
	physics->SetPhysicsPreset(PhysicsPreset::PLATFORMER_PHYSICS_PRESET);
	physics->SetScenarioPreset(ScenarioPreset::PLATFORMER_1280x720_SCENARIO_PRESET);
}

void Scene::UpdateLogoScene(float dt)
{
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	float x = (float)mouseX;
	float y = (float)mouseY;

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		test = nullptr;
		//test = (DynamicBody*)physics->CreateBody(BodyType::DYNAMIC_BODY, fPoint{ x, y }, CircleCollider{ x, y, 40,}, { 0, 0 }, { 0, 0 }, 1.5f);
		test = (DynamicBody*)physics->CreateBody(BodyType::DYNAMIC_BODY, fPoint{ x, y }, { mouseX, mouseY, 20, 40 }, { 0, 0 }, { 0, 0 }, 1.5f);
		test->ResetForces();
	}

	physics->Update(dt);

	// BUG SOLVINGS
	// (Known problem, dt++ when frame frozzen, then y++++ for gravity/velocity (and for x forces/velocity too)) / Death limit bug - Not solved but info: It happens in the function "Death limit". When camera is mover, y axis of body boost the hell out and it detects outside the limits
	// End static body bug - When a dinamic body approaches the limits of a static body, it automatically teleports to the limit	
	// (trerballar amb velocity * massa) Wall jump bugg - Gravity affects a lot on walljump, so at the 2/3rd jump you can not wall jump more because the force applied is so low. How to solve that?

	// NEW BUG
	// Double jump jank. If double jump fast, going higher, else, not higher
	// SPHERE COLLISION GROUND

	// TO IMPLEMENT
	// Wall friction when walljump active
	// RPG physics/scenario

	// FUTURE IMPLEMENTATION
	// Implement join: "Rope", fero si o si ELASTIC -> Future Bug, Elastic + Velocity 8 = the end of the actual game :) + feature
	// Velocity Limit library, + boleans per activar/desactivar cada un per utilitzarlos a la vegada si vull
	// Wall jump library (real (com el que tinc ara), no realistic...)
	// Add raycasing
	// Add water rect (onWater / onAir bools)

	physics->DeathLimit({ 0,0,1280,720 });

	if (test != nullptr)
	{
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			test->Jump(-300.0f, true);
			test->WallJump({ 300.0f, 350.0f });
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) test->Move(5.0f, Direction::LEFT, 300.0f);
			else test->Move(5.0f, Direction::LEFT, 200.0f);

			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
			{
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) test->Dash(400.0f, DashDirection::UPLEFT, 1);
				else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) test->Dash(400.0f, DashDirection::DOWNLEFT, 1);
				else test->Dash(400.0f, DashDirection::LEFT, 1);
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) test->Move(5.0f, Direction::RIGHT, 300.0f);
			else test->Move(5.0f, Direction::RIGHT, 200.0f);

			if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
			{
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) test->Dash(400.0f, DashDirection::UPRIGHT, 1);
				else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) test->Dash(400.0f, DashDirection::DOWNRIGHT, 1);
				else test->Dash(400.0f, DashDirection::RIGHT, 1);
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) test->Dash(400.0f, DashDirection::DOWN, 1);

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) test->Dash(400.0f, DashDirection::UP, 1);
	}

	physics->Draw(test);

	//if (test != nullptr)
	//{
	//	fPoint pos = test->GetPosition();
	//	LOG("BodyX: %.3f BodyY: %.3f", pos.x, pos.y);
	//}
}

// GUI CONTROLS

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (currScene)
	{
	case LOGO_SCENE:
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
	}
}
