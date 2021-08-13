#include "App.h"
#include "EntityManager.h"

#include "LevelEditorScene.h"
#include "Physics.h"
#include "Player.h"
#include "Input.h"
#include "Render.h"
#include "GroundTile.h"

#include "Log.h"

LevelEditor::LevelEditor()
{
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::Start()
{
	phys = new Physics();
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->body = (DynamicBody*)phys->CreateBody(BodyType::DYNAMIC_BODY, fPoint{ 300.0f, 700.0f }, { 300, 1000, 46, 53 }, { 0, 0 }, { 0, 0 }, 1.5f);
	phys->SetPhysicsPreset(PhysicsPreset::PLATFORMER_PHYSICS_PRESET);
	phys->PausePhysics(true);
	
	app->render->camera = {INIT_CAM_X, -INIT_CAM_Y };
	player->Start();
	state = EditorState::EDITING;

	background[0] = app->tex->Load("Assets/Textures/Background/sky_bg.png");
	background[1] = app->tex->Load("Assets/Textures/Background/sky_move1_bg.png");
	background[2] = app->tex->Load("Assets/Textures/Background/sky_move2_bg.png");
}

void LevelEditor::Update(float dt)
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
		UpdateEditor(dt);
		break;

	case EditorState::PLAYING:
		UpdatePreview(dt);
		break;

	case EditorState::PAUSE_MENU:
		break;
	}
}

void LevelEditor::Draw()
{
	DrawBackground();

	ListItem<Tile*>* list;
	for (list = tiles.start; list != nullptr; list = list->next) list->data->Draw();

	DebugDraw();
	phys->Draw(player->body);
}

void LevelEditor::DrawBackground()
{
	unsigned short int bgSize = 2608;
	unsigned short int camWidth = 1280;
	SDL_Rect bgEnd = { 0, 0, 2297, 917 };

	short int lowVel = app->render->camera.x / 25;
	short int midVel = app->render->camera.x / 15;
	int camX = -app->render->camera.x;

	if (camX < bgSize) app->render->DrawTexture(background[0], (bgSize * 0), 0, 1, 1, (SDL_Rect*)0, false);
	if (camX > ((bgSize * 1) - camWidth) && camX < (bgSize * 2)) app->render->DrawTexture(background[0], (bgSize * 1), 0, 1, 1, (SDL_Rect*)0, false);
	if (camX > ((bgSize * 2) - camWidth) && camX < (bgSize * 3)) app->render->DrawTexture(background[0], (bgSize * 2), 0, 1, 1, (SDL_Rect*)0, false);
	if (camX > ((bgSize * 3) - camWidth) && camX < (bgSize * 4)) app->render->DrawTexture(background[0], (bgSize * 3), 0, 1, 1, (SDL_Rect*)0, false);
	if (camX > ((bgSize * 4) - camWidth) && camX < (bgSize * 5)) app->render->DrawTexture(background[0], (bgSize * 4), 0, 1, 1, &bgEnd, false);

	if (camX < 2550) app->render->DrawTexture(background[1], lowVel, 0, 1, 1, (SDL_Rect*)0, false);
	if (camX > 1278 && camX < 4200) app->render->DrawTexture(background[1], lowVel + (bgSize * 1), 0, 1, 1, (SDL_Rect*)0, false);
	if (camX > 3788 && camX < 8000) app->render->DrawTexture(background[1], lowVel + (bgSize * 2), 0, 1, 1, (SDL_Rect*)0, false);

	app->render->DrawTexture(background[2], midVel, 0, 1, 1, (SDL_Rect*)0, false);

	LOG("X: %d", -camX);
}

void LevelEditor::DebugDraw()
{
	Uint8 alpha = 230 / ((Uint8)state + 1);
	for (int i = 0; i < wTileScreen[currScreen + 1]; i++) app->render->DrawLine((i + 1) * TILE_SIZE, MAX_SCREEN_Y, (i + 1) * TILE_SIZE, 0, {100, 100, 100, alpha });
	for (int i = 0; i < TILE_PER_SCREEN_H; i++) app->render->DrawLine(MAX_SCREEN_X, i * TILE_SIZE, 0, i * TILE_SIZE, { 100, 100, 100, alpha });
}

void LevelEditor::CleanUp()
{
	delete phys;
	delete player->body;
	delete player;

	for (int i = 0; i < 3; i++) app->tex->UnLoad(background[i]);
}

//---------------------------------------------------------------------------------

void LevelEditor::UpdateEditor(float dt)
{
	TileSelectionLogic();

	CameraDisplace();
	ScreenAddition();
	PlayerDragLogic();

	TilePlacement();
	TileRemoveLogic();
}

void LevelEditor::TileSelectionLogic()
{
	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) tSelect = TileSelect::NO_SELECT;
	if (app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) tSelect = TileSelect::ERASE;
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) tSelect = TileSelect::GROUND;
}

void LevelEditor::CameraDisplace()
{
	fPoint pos = player->body->GetPosition();

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		if (app->render->camera.y < 0)
		{
			app->render->camera.y += CAM_VEL;
			player->UpdatePosition({(int)pos.x, (int)pos.y - CAM_VEL});
			pos = player->body->GetPosition();
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		if (app->render->camera.y > -INIT_CAM_Y)
		{
			app->render->camera.y -= CAM_VEL;
			player->UpdatePosition({ (int)pos.x, (int)pos.y + CAM_VEL });
			pos = player->body->GetPosition();
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (app->render->camera.x < 0)
		{
			app->render->camera.x += CAM_VEL;
			player->UpdatePosition({ (int)pos.x - CAM_VEL, (int)pos.y});
			pos = player->body->GetPosition();
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (app->render->camera.x > -(TILE_SIZE * (wTileScreen[currScreen])))
		{
			app->render->camera.x -= CAM_VEL;
			player->UpdatePosition({ (int)pos.x + CAM_VEL, (int)pos.y });
		}
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

void LevelEditor::TilePlacement()
{
	if ((int)tSelect > 1 && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		iPoint coord = GetCoordsFromMousePos();
		GroundTile* gT = nullptr;

		if (TileExistance(coord)) return;

		switch (tSelect)
		{
		case TileSelect::GROUND:
			gT = new GroundTile({float(coord.x * TILE_SIZE), float(coord.y * TILE_SIZE)}, coord, this);
			tiles.Add(gT);
			break;
		}
	}
}

void LevelEditor::TileRemoveLogic()
{
	if (tSelect == TileSelect::ERASE && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		iPoint coord = GetCoordsFromMousePos();

		if (!TileExistance(coord)) return;

		DeleteTile(coord);
	}
}

iPoint LevelEditor::GetCoordsFromMousePos()
{
	iPoint pos = {};
	iPoint coords = {};
	app->input->GetMousePosition(pos.x, pos.y);

	pos.x -= app->render->camera.x;
	pos.y -= app->render->camera.y;

	if (pos.x < 0) coords.x = floor(pos.x / TILE_SIZE) - 1;
	else coords.x = floor(pos.x / TILE_SIZE);

	if (pos.y < 0) coords.y = floor(pos.y / TILE_SIZE) - 1;
	else coords.y = floor(pos.y / TILE_SIZE);

	return coords;
}

bool LevelEditor::TileExistance(iPoint coords)
{
	ListItem<Tile*>* list;
	for (list = tiles.start; list != nullptr; list = list->next) if (list->data->coordinates == coords) return true;

	return false;
}

void LevelEditor::DeleteTile(iPoint coords)
{
	ListItem<Tile*>* list;
	for (list = tiles.start; list != nullptr; list = list->next)
	{
		if (list->data->coordinates == coords)
		{
			phys->DestroyBody(list->data->GetBody());
			delete list->data;
			tiles.Del(list);
			return;
		}
	}
	return;
}

void LevelEditor::PlayerDragLogic()
{
	iPoint pos = {};
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && IsMouseInPlayer())
	{
		tSelect = TileSelect::NO_SELECT;
		player->dragged = true;

		// GetMousePosInTile
		app->input->GetMousePosition(pos.x, pos.y);
		utils.ApplyCameraPosition(pos.x, pos.y);
		
		deltaPosition = GetMousePosInPlayer(pos);
		pos -= deltaPosition;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && player->dragged)
	{
		app->input->GetMousePosition(pos.x, pos.y);
		utils.ApplyCameraPosition(pos.x, pos.y);

		pos -= deltaPosition;
	}
	else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && player->dragged) player->dragged = false;

	if (player->dragged)
	{
		SDL_Rect cam = app->render->camera;
		if (utils.CheckCollision(player->body->ReturnBodyRect(), {-cam.x, -cam.y, 1280, 720})) player->UpdatePosition(pos);

		if (player->body->GetPosition().x < -cam.x) player->UpdatePosition({-cam.x, pos.y});

		int maxW = -cam.x + 1280 - player->body->GetMagnitudes().x;
		if (player->body->GetPosition().x > maxW) player->UpdatePosition({ maxW, pos.y });

		if (player->body->GetPosition().y < -cam.y) player->UpdatePosition({ pos.x, -cam.y });

		int maxH = -cam.y + 720 - player->body->GetMagnitudes().y;
		if (player->body->GetPosition().y > maxH) player->UpdatePosition({ pos.x, maxH });
	}
}

bool LevelEditor::IsMouseInPlayer()
{
	iPoint pos = {};
	app->input->GetMousePosition(pos.x, pos.y);
	utils.ApplyCameraPosition(pos.x, pos.y);

	return utils.CheckCollision(player->body->ReturnBodyRect(), {pos.x, pos.y, 1, 1});
}

iPoint LevelEditor::GetMousePosInPlayer(iPoint pos)
{
	return iPoint(pos.x - (int)player->body->GetPosition().x, pos.y - (int)player->body->GetPosition().y);
}

void LevelEditor::UpdatePreview(float dt)
{
	player->Update(dt);
}
