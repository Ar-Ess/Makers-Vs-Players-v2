#include "App.h"
#include "EntityManager.h"

#include "LevelEditorScene.h"
#include "Physics.h"
#include "Player.h"
#include "Input.h"
#include "Render.h"
#include "GroundTile.h"
#include "Coin.h"
#include "BrickTile.h"
#include "SpikeTrapTile.h"
#include "ArrowSignTile.h"

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
	player->body = (DynamicBody*)phys->CreateBody(BodyType::DYNAMIC_BODY, fPoint{ 300.0f, 700.0f }, { 300, 1000, PLAYER_W, PLAYER_H }, { 0, 0 }, { 0, 0 }, 1.5f);
	if (!phys->SetBodyAsPlayer(player->body)) LOG("PLAYER BODY IS NOT SETTED AS PLAYER");

	phys->SetPhysicsPreset(PhysicsPreset::PLATFORMER_PHYSICS_PRESET);
	phys->PausePhysics(true);
	
	app->render->camera = {INIT_CAM_X, -INIT_CAM_Y };
	player->Start();
	state = EditorState::EDITING;
	bg = Background::SKY;
	menu = true;

	LoadBackgroundImages();
}

void LevelEditor::Update(float dt)
{
	phys->Update(dt);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		switch (state)
		{
		case EditorState::EDITING: 
			ChangeEditorState(EditorState::PLAYING); 
			break;

		case EditorState::PLAYING: 
			ChangeEditorState(EditorState::EDITING); 
			break;
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

void LevelEditor::Draw(float dt)
{
	DrawBackground();

	for (ListItem<Tile*>* list = tiles.start; list != nullptr; list = list->next) if (list->data->zAxis == Z::BACK) list->data->Draw(dt);
	for (ListItem<Tile*>* list = tiles.start; list != nullptr; list = list->next) if (list->data->zAxis == Z::MID) list->data->Draw(dt);
	for (ListItem<Tile*>* list = tiles.start; list != nullptr; list = list->next) if (list->data->zAxis == Z::FRONT) list->data->Draw(dt);

	player->Draw(dt);
	DebugDraw();
	phys->Draw(player->body);
}

void LevelEditor::DrawBackground()
{
	unsigned short int bgSize = 2608;
	unsigned short int camWidth = 1280;
	SDL_Rect bgEnd = { 0, 0, 2297, 917 };
	SDL_Rect bg1End = { 0, 0, 145, 1106 };
	SDL_Rect bg2End = { 0, 0, 451, 1431 };

	short int lowVel = app->render->camera.x / 25;
	short int midVel = app->render->camera.x / 15;
	short int sunVel = (-app->render->camera.x / 1.245f) + 400;

	int camX = -app->render->camera.x;
	int camYmid = (app->render->camera.y + INIT_CAM_Y) / 9;
	int camYlow = (app->render->camera.y + INIT_CAM_Y) / 13;

	if (camX < bgSize) app->render->DrawTexture(background[0], (bgSize * 0), 0, 1, 1, (SDL_Rect*)0, false);
	if (camX > ((bgSize * 1) - camWidth) && camX < (bgSize * 2)) app->render->DrawTexture(background[0], (bgSize * 1), 0, 1, 1, (SDL_Rect*)0, false);
	if (camX > ((bgSize * 2) - camWidth) && camX < (bgSize * 3)) app->render->DrawTexture(background[0], (bgSize * 2), 0, 1, 1, (SDL_Rect*)0, false);
	if (camX > ((bgSize * 3) - camWidth) && camX < (bgSize * 4)) app->render->DrawTexture(background[0], (bgSize * 3), 0, 1, 1, (SDL_Rect*)0, false);
	if (camX > ((bgSize * 4) - camWidth) && camX < (bgSize * 5)) app->render->DrawTexture(background[0], (bgSize * 4), 0, 1, 1, &bgEnd, false);

	if (bg == Background::NOON) app->render->DrawTexture(background[3], sunVel, -camYmid, 1, 1, (SDL_Rect*)0, false);

	if (camX < 2550) app->render->DrawTexture(background[1], lowVel, -camYlow, 1, 1, (SDL_Rect*)0, false);
	if (camX > 1278 && camX < 5020) app->render->DrawTexture(background[1], lowVel + (bgSize * 1), -camYlow, 1, 1, (SDL_Rect*)0, false);
	if (camX > 3788 && camX < 7560) app->render->DrawTexture(background[1], lowVel + (bgSize * 2), -camYlow, 1, 1, (SDL_Rect*)0, false);
	if (camX > 6291 && camX < 10030) app->render->DrawTexture(background[1], lowVel + (bgSize * 3), -camYlow, 1, 1, (SDL_Rect*)0, false);
	if (camX > 8802) app->render->DrawTexture(background[1], lowVel + (bgSize * 4), -camYlow, 1, 1, (SDL_Rect*)0, false);
	if (camX > 11304) app->render->DrawTexture(background[1], lowVel + (bgSize * 5), -camYlow, 1, 1, &bg1End, false);

	if (camX < 2500) app->render->DrawTexture(background[2], midVel, camYmid, 1, 1, (SDL_Rect*)0, false);
	if (camX > 1242 && camX < 4920) app->render->DrawTexture(background[2], midVel + (bgSize * 1), camYmid, 1, 1, (SDL_Rect*)0, false);
	if (camX > 3690 && camX < 7410) app->render->DrawTexture(background[2], midVel + (bgSize * 2), camYmid, 1, 1, (SDL_Rect*)0, false);
	if (camX > 6129 && camX < 9830) app->render->DrawTexture(background[2], midVel + (bgSize * 3), camYmid, 1, 1, (SDL_Rect*)0, false);
	if (camX > 8568) app->render->DrawTexture(background[2], midVel + (bgSize * 4), camYmid, 1, 1, (SDL_Rect*)0, false);
	if (camX > 11025) app->render->DrawTexture(background[2], midVel + (bgSize * 5), camYmid, 1, 1, &bg2End, false);
}

void LevelEditor::DebugDraw()
{
	Uint8 alpha = 230 / ((Uint8)state + 1);
	for (int i = 0; i < wTileScreen[currScreen + 1]; i++) app->render->DrawLine((i + 1) * TILE_SIZE, MAX_SCREEN_Y, (i + 1) * TILE_SIZE, 0, {100, 100, 100, alpha });
	for (int i = 0; i < TILE_PER_SCREEN_H; i++) app->render->DrawLine(MAX_SCREEN_X, i * TILE_SIZE, 0, i * TILE_SIZE, { 100, 100, 100, alpha });
	
	if (menu)
	{
		SDL_Rect cam = app->render->camera;
		app->render->DrawRectangle({ -cam.x, -cam.y, 1280, 100 }, { 255, 0, 0, 100 });
	}
}

void LevelEditor::CleanUp()
{
	delete phys;
	delete player->body;
	delete player;

	for (int i = 0; i < 4; i++)
	{
		app->tex->UnLoad(background[i]);
		background[i] = nullptr;
	}
}

void LevelEditor::LoadBackgroundImages()
{
	switch (bg)
	{
	case Background::SKY:
		background[0] = app->tex->Load("Assets/Textures/Background/sky_bg.png");
		background[1] = app->tex->Load("Assets/Textures/Background/sky_move1_bg.png");
		background[2] = app->tex->Load("Assets/Textures/Background/sky_move2_bg.png");
		background[3] = nullptr;
		break;

	case Background::NOON:
		background[0] = app->tex->Load("Assets/Textures/Background/noon_bg.png");
		background[1] = app->tex->Load("Assets/Textures/Background/noon_move1_bg.png");
		background[2] = app->tex->Load("Assets/Textures/Background/noon_move2_bg.png");
		background[3] = app->tex->Load("Assets/Textures/Background/noon_sun_bg.png");
		break;
	}
}

void LevelEditor::ChangeEditorState(EditorState newState)
{
	ListItem<Tile*>* list;
	switch (newState)
	{
	case EditorState::EDITING:
		phys->PausePhysics(true);
		for (list = tiles.start; list != nullptr; list = list->next) list->data->Restart();
		state = newState;
		break;

	case EditorState::PLAYING:
		phys->PausePhysics(false);
		phys->ResetAllForces();
		state = newState;
		break;

	case EditorState::PAUSE_MENU:
		break;
	}
}

//---------------------------------------------------------------------------------

void LevelEditor::UpdateEditor(float dt)
{
	TileSelectionLogic();
	ChangeTilesetLogic();

	CameraDisplace();
	ScreenAddition();
	PlayerDragLogic();
	RotationLogic();

	TileMenuLogic();
	TilePlaceLogic();
	TileRemoveLogic();
}

void LevelEditor::TileSelectionLogic()
{
	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) select = Selection::NO_SELECT;
	if (app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) select = Selection::ERASE;
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) select = Selection::GROUND;
	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) select = Selection::COIN;
	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) select = Selection::BRICK;
	if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) select = Selection::SPIKE_TRAP;
	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) select = Selection::ARROW_SIGN;
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
		ScreenRemoving(currScreen);

		currScreen--;
		int newCamPos = wTileScreen[currScreen] * TILE_SIZE;
		int deltaCamPLayer = (int)player->body->GetPosition().x + app->render->camera.x;

		app->render->camera.x = -newCamPos;
		player->UpdatePosition({ (-app->render->camera.x) + deltaCamPLayer, (int)player->body->GetPosition().y });
	}
}

void LevelEditor::ScreenRemoving(int screen)
{
	for (int x = wTileScreen[screen]; x < wTileScreen[screen + 1]; x++)
	{
		for (int y = 0; y < TILE_PER_SCREEN_H; y++)
		{
			if (TileExistance(iPoint{ x, y })) DeleteTileLogic(iPoint{ x, y });
		}
	}
}

void LevelEditor::TileMenuLogic()
{
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) menu = !menu;
}

void LevelEditor::TilePlaceLogic()
{
	if ((int)select > 1 && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		iPoint coord = GetCoordsFromMousePos();

		if (TileExistance(coord)) return;

		switch (select)
		{
		case Selection::GROUND:
		{
			GroundTile* gT = new GroundTile({ float(coord.x * TILE_SIZE), float(coord.y * TILE_SIZE) }, coord, this, Z::MID);
			tiles.Add(gT);
			break;
		}
		case Selection::COIN:
		{
			Coin* c = new Coin({ float(coord.x * TILE_SIZE), float(coord.y * TILE_SIZE) }, coord, this, Z::FRONT);
			tiles.Add(c);
			break;
		}
		case Selection::BRICK:
		{
			BrickTile* bT = new BrickTile({ float(coord.x * TILE_SIZE), float(coord.y * TILE_SIZE) }, coord, this, Z::MID);
			tiles.Add(bT);
			break;
		}
		case Selection::SPIKE_TRAP:
		{
			SpikeTrapTile* sTT = new SpikeTrapTile({ float(coord.x * TILE_SIZE), float(coord.y * TILE_SIZE) }, coord, this, Z::MID);
			tiles.Add(sTT);
			break;
		}
		case Selection::ARROW_SIGN:
		{
			ArrowSignTile* aST = new ArrowSignTile({ float(coord.x * TILE_SIZE), float(coord.y * TILE_SIZE) }, coord, this, Z::BACK, arrowAngle);
			tiles.Add(aST);
			break;
		}
		}
	}
}

void LevelEditor::TileRemoveLogic()
{
	if (select == Selection::ERASE && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		iPoint coord = GetCoordsFromMousePos();

		if (!TileExistance(coord)) return;

		DeleteTileLogic(coord);
	}
}

void LevelEditor::RotationLogic()
{
	if (select == Selection::ARROW_SIGN)
	{
		if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			if (arrowAngle != 7) arrowAngle++;
			else arrowAngle = 0;
		}
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

Tile* LevelEditor::GetTileFromCoords(iPoint coords)
{
	for (int i = 0; i < tiles.Count(); i++)
	{
		if (tiles.At(i)->data->coordinates == coords) return tiles.At(i)->data;
	}
	return nullptr;
}

bool LevelEditor::TileExistance(iPoint coords)
{
	ListItem<Tile*>* list;
	for (list = tiles.start; list != nullptr; list = list->next)
	{
		if (list->data->fourByFour)
		{
			SDL_Rect newArrow = { coords.x, coords.y, 2, 2 };
			SDL_Rect oldArrow = { list->data->coordinates.x, list->data->coordinates.y, 2, 2 };

			if (utils.CheckCollisionLimits(newArrow, oldArrow)) return true;
		}
		else
		{
			if (list->data->coordinates == coords) return true;
		}
	}

	return false;
}

void LevelEditor::DeleteTileLogic(iPoint coords)
{
	ListItem<Tile*>* list;
	for (list = tiles.start; list != nullptr; list = list->next)
	{
		if (list->data->fourByFour)
		{
			bool exist = false;
			SDL_Rect newArrow = { coords.x, coords.y, 1, 1 };
			SDL_Rect oldArrow = { list->data->coordinates.x, list->data->coordinates.y, 2, 2 };

			if (utils.CheckCollisionLimits(newArrow, oldArrow)) exist = true;

			if (exist)
			{
				if (list->data->GetBody() != nullptr) phys->DestroyBody(list->data->GetBody());
				delete list->data;
				tiles.Del(list);
				return;
			}
		}
		else
		{
			if (list->data->coordinates == coords)
			{
				if (list->data->GetBody() != nullptr) phys->DestroyBody(list->data->GetBody());
				delete list->data;
				tiles.Del(list);
				return;
			}
		}
	}
	return;
}

void LevelEditor::PlayerDragLogic()
{
	iPoint pos = {};
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && IsMouseInPlayer())
	{
		select = Selection::NO_SELECT;
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

void LevelEditor::ChangeTilesetLogic()
{
	if (app->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_DOWN)
	{
		bg = (Background)(((int)bg) + 1);
		if ((int)bg > MAX_BACKGROUND) bg = (Background)1;

		for (int i = 0; i < 4; i++)
		{
			app->tex->UnLoad(background[i]);
			background[i] = nullptr;
		}

		ListItem<Tile*>* item;
		for (item = tiles.start; item != NULL; item = item->next)
		{
			app->tex->UnLoad(item->data->texture);
			item->data->texture = nullptr;

			switch (bg)
			{
			case Background::SKY:
				switch (item->data->type)
				{
				case TileType::GROUND:
					item->data->texture = app->tex->Load("Assets/Textures/Tilesets/ground_sky_tileset.png");
					break;

				case TileType::COIN:
					item->data->texture = app->tex->Load("Assets/Textures/Items/coin_sky_item.png");
					break;

				case TileType::BRICK:
					item->data->texture = app->tex->Load("Assets/Textures/Tilesets/brick_sky_tileset.png");
					break;

				case TileType::SPIKE_TRAP:
					item->data->texture = app->tex->Load("Assets/Textures/Tilesets/spike_trap_sky_tileset.png");
					break;
				}
				break;

			case Background::NOON:
				switch (item->data->type)
				{
				case TileType::GROUND:
					item->data->texture = app->tex->Load("Assets/Textures/Tilesets/ground_noon_tileset.png");
					break;

				case TileType::COIN:
					item->data->texture = app->tex->Load("Assets/Textures/Items/coin_noon_item.png");
					break;

				case TileType::BRICK:
					item->data->texture = app->tex->Load("Assets/Textures/Tilesets/brick_noon_tileset.png");
					break;

				case TileType::SPIKE_TRAP:
					item->data->texture = app->tex->Load("Assets/Textures/Tilesets/spike_trap_sky_tileset.png");
					break;
				}
				break;
			}
		}
		LoadBackgroundImages();
	}
}

void LevelEditor::UpdatePreview(float dt)
{
	player->Update(dt);

	ListItem<Tile*>* list;
	for (list = tiles.start; list != nullptr; list = list->next) list->data->Update(dt);

	//ALWAYS END
	OutBoundsDeath();
}

void LevelEditor::OutBoundsDeath()
{
	iPoint pos = { (int)player->body->GetPosition().x, (int)player->body->GetPosition().y };
	if (pos.y > -app->render->camera.y + 720 + 100)
	{
		PreviewDeath();
	}
}

void LevelEditor::PreviewDeath()
{
	iPoint pos = { (int)player->body->GetPosition().x, (int)player->body->GetPosition().y };
	ChangeEditorState(EditorState::EDITING);
	player->UpdatePosition({ pos.x, -app->render->camera.y + 360 });
}

// BUGS
// 1. When placing a coin on top of a solid block, the block collision is weird, player clips through that block