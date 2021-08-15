#include "BrickTile.h"

#include "App.h"
#include "Render.h"
#include "Scene.h"
#include "LevelEditorScene.h"
#include "Textures.h"

BrickTile::BrickTile()
{
}

BrickTile::BrickTile(fPoint pos, iPoint coords, LevelEditor* lE)
{
	switch (lE->GetBackground())
	{
	case Background::SKY:
		texture = app->tex->Load("Assets/Textures/Tilesets/brick_sky_tileset.png");
		break;

	case Background::NOON:
		texture = app->tex->Load("Assets/Textures/Tilesets/brick_noon_tileset.png");
		break;
	}

	position = pos;
	coordinates = coords;
	editor = lE;
	type = TileType::BRICK;
	state = BrickState::BRICK;
	body = (StaticBody*)editor->phys->CreateBody(BodyType::STATIC_BODY, pos, { (int)pos.x, (int)pos.y, TILE_SIZE, TILE_SIZE });

	rect = { (int)pos.x + 12, (int)pos.y + 6, 53 - 24, 53 - 12 };

	for (int i = 0; i < 9; i++) coinAnim.PushBack({ 53 * i, 0, 53, 53 });
	coinAnim.loop = true;
	coinAnim.speed = 12.0f;
}

BrickTile::~BrickTile()
{
	delete body;
}

void BrickTile::Update(float dt)
{
	if (state != BrickState::COLLECTED && !pSwitch && app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		pSwitch = true;
		state = BrickState::COIN;
		body->SetCollidable(false);
		app->tex->UnLoad(texture);
		switch (editor->GetBackground())
		{
		case Background::SKY:
			texture = app->tex->Load("Assets/Textures/Items/coin_sky_item.png");
			break;

		case Background::NOON:
			texture = app->tex->Load("Assets/Textures/Items/coin_noon_item.png");
			break;
		}
	}

	if (pSwitch)
	{
		if (timer > P_SWITCH_TIME)
		{
			pSwitch = false;
			timer = 0;
			if (state != BrickState::COLLECTED) state = BrickState::BRICK;
			body->SetCollidable(true);
			app->tex->UnLoad(texture);
			switch (editor->GetBackground())
			{
			case Background::SKY:
				texture = app->tex->Load("Assets/Textures/Tilesets/brick_sky_tileset.png");
				break;

			case Background::NOON:
				texture = app->tex->Load("Assets/Textures/Tilesets/brick_noon_tileset.png");
				break;
			}
		}
		else timer++;
	}
}

void BrickTile::Draw(float dt)
{
	bool debug = editor->phys->debug;
	iPoint leftTile = {};
	iPoint rightTile = {};
	bool left = false;
	bool right = false;
	ListItem<Tile*>* list;

	switch (state)
	{
	case BrickState::COIN:
		if (editor->GetState() == EditorState::PLAYING) coinAnim.Update(dt);
		else if (editor->GetState() == EditorState::EDITING) coinAnim.Reset();
		app->render->DrawTexture(texture, position.x, position.y, 1, 1, &coinAnim.GetCurrentFrame(), false);

		if (debug) app->render->DrawRectangle(rect, { 200, 200, 0, 150 });
		break;

	case BrickState::BRICK:
		leftTile = { coordinates.x - 1, coordinates.y };
		rightTile = { coordinates.x + 1, coordinates.y };

		for (list = editor->tiles.start; list != nullptr; list = list->next)
		{
			if (list->data->type == TileType::BRICK)
			{
				if (list->data->coordinates == leftTile) left = true;
				if (list->data->coordinates == rightTile) right = true;

				if (left && right) break;
			}
		}

		if (left && !right) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[2]);
		else if (!left && right) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[1]);
		else if (!left && !right) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[0]);
		else if (left && right) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[3]);
		break;

	case BrickState::COLLECTED:
		break;
	}
}

void BrickTile::Restart()
{
	pSwitch = false;
	timer = 0;
	if (state == BrickState::COIN)
	{
		body->SetCollidable(true);
		app->tex->UnLoad(texture);
		switch (editor->GetBackground())
		{
		case Background::SKY:
			texture = app->tex->Load("Assets/Textures/Tilesets/brick_sky_tileset.png");
			break;

		case Background::NOON:
			texture = app->tex->Load("Assets/Textures/Tilesets/brick_noon_tileset.png");
			break;
		}
	}

	state = BrickState::BRICK;
}
