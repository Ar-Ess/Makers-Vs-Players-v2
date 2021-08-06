#include "LevelEditorScene.h"
#include "Physics.h"
#include "Player.h"
#include "Input.h"
#include "Render.h"

LevelEditor::LevelEditor()
{
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::Start()
{
	app->render->camera = {INIT_CAM_X, -INIT_CAM_Y };
	state = EditorState::EDITING;
	
}

void LevelEditor::Update(float dt, Player* p, Physics* phys)
{
	phys->Update(dt);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if ((int)state == 0)
		{
			phys->PausePhysics(false);
			phys->ResetAllForces();
			state = EditorState::PLAYING;
		}
		else if ((int)state == 1)
		{
			phys->PausePhysics(true);
			state = EditorState::EDITING;
		}
	}

	switch (state)
	{
	case EditorState::EDITING:
		UpdateEditor(p);
		break;

	case EditorState::PLAYING:
		UpdatePreview(p);
		break;

	case EditorState::PAUSE_MENU:
		break;
	}

}

void LevelEditor::Draw(Player* p, Physics* phys)
{
	phys->Draw(p->body);
	DebugDraw();
}

void LevelEditor::DebugDraw()
{
	Uint8 alpha = 250 / ((Uint8)state + 1);
	for (int i = 0; i < wTileScreen[currScreen]; i++) app->render->DrawLine((i + 1) * TILE_SIZE, MAX_SCREEN_Y, (i + 1) * TILE_SIZE, 0, {100, 100, 100, alpha });
	for (int i = 0; i < hTileScreen; i++) app->render->DrawLine(MAX_SCREEN_X, i * TILE_SIZE, 0, i * TILE_SIZE, { 100, 100, 100, alpha });
}

void LevelEditor::CleanUp()
{
}

//---------------------------------------------------------------------------------

void LevelEditor::UpdateEditor(Player* p)
{
	CameraDisplace();
}

void LevelEditor::CameraDisplace()
{
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		app->render->camera.y -= CAM_VEL;
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		app->render->camera.y += CAM_VEL;
	}

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		app->render->camera.x -= CAM_VEL;
	}

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		app->render->camera.x += CAM_VEL;
	}
}

void LevelEditor::UpdatePreview(Player* p)
{
	p->PlayerControls();
}
