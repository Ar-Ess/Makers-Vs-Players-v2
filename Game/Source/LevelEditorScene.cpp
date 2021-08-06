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
	Uint8 alpha = 230 / ((Uint8)state + 1);
	for (int i = 0; i < wTileScreen[currScreen + 1]; i++) app->render->DrawLine((i + 1) * TILE_SIZE, MAX_SCREEN_Y, (i + 1) * TILE_SIZE, 0, {100, 100, 100, alpha });
	for (int i = 0; i < TILE_PER_SCREEN_H; i++) app->render->DrawLine(MAX_SCREEN_X, i * TILE_SIZE, 0, i * TILE_SIZE, { 100, 100, 100, alpha });
}

void LevelEditor::CleanUp()
{
}

//---------------------------------------------------------------------------------

void LevelEditor::UpdateEditor(Player* p)
{
	CameraDisplace();
	ScreenAddition();
}

void LevelEditor::CameraDisplace()
{
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (app->render->camera.y < 0) app->render->camera.y += CAM_VEL;
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (app->render->camera.y > -INIT_CAM_Y) app->render->camera.y -= CAM_VEL;
	}

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (app->render->camera.x < 0) app->render->camera.x += CAM_VEL;
	}

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (app->render->camera.x > -(TILE_SIZE * (wTileScreen[currScreen]))) app->render->camera.x -= CAM_VEL;
	}
}

void LevelEditor::ScreenAddition()
{
	if (app->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN && currScreen < 9) currScreen++;

	if (app->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN && currScreen > 0)
	{
		currScreen--;
		app->render->camera.x = -wTileScreen[currScreen] * TILE_SIZE;
	}
}

void LevelEditor::UpdatePreview(Player* p)
{
	p->PlayerControls();
}
