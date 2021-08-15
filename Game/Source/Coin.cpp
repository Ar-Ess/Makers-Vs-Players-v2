#include "Coin.h"
#include "Input.h"
#include "LevelEditorScene.h"
#include "Physics.h"
#include "Player.h"

Coin::Coin()
{
}

Coin::Coin(fPoint pos, iPoint coords, LevelEditor* lE)
{
	position = pos;
	coordinates = coords;
	type = TileType::COIN;
	state = CoinState::COIN;
	editor = lE;

	switch (lE->GetBackground())
	{
	case Background::SKY:
		texture = app->tex->Load("Assets/Textures/Items/coin_sky_item.png");
		break;

	case Background::NOON:
		texture = app->tex->Load("Assets/Textures/Items/coin_noon_item.png");
		break;
	}

	body = (StaticBody*)editor->phys->CreateBody(BodyType::STATIC_BODY, pos, { (int)pos.x, (int)pos.y, TILE_SIZE, TILE_SIZE });
	body->SetCollidable(false);

	rect = {(int)pos.x + 12, (int)pos.y + 6, 53 - 24, 53 - 12};

	for (int i = 0; i < 9; i++) coinAnim.PushBack({53 * i, 0, 53, 53});
	coinAnim.loop = true;
	coinAnim.speed = 12.0f;
}

Coin::~Coin()
{
}

void Coin::Update(float dt) // Function only reached when preview state
{
	if (state == CoinState::COIN && utils.CheckCollision(rect, editor->player->body->ReturnBodyRect())) state = CoinState::COLLECTED;

	if (state != CoinState::COLLECTED && !pSwitch && app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		pSwitch = true;
		state = CoinState::BRICK;
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

	if (pSwitch)
	{
		if (timer > P_SWITCH_TIME)
		{
			pSwitch = false;
			timer = 0;
			if (state != CoinState::COLLECTED) state = CoinState::COIN;
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
		else timer++;
	}
}

void Coin::Draw(float dt)
{
	bool debug = editor->phys->debug;
	iPoint leftTile = {};
	iPoint rightTile = {};
	bool left = false;
	bool right = false;
	ListItem<Tile*>* list;

	switch (state)
	{
	case CoinState::COIN:
		if (editor->GetState() == EditorState::PLAYING) coinAnim.Update(dt);
		else if (editor->GetState() == EditorState::EDITING) coinAnim.Reset();
		app->render->DrawTexture(texture, position.x, position.y, 1, 1, &coinAnim.GetCurrentFrame(), false);

		if (debug) app->render->DrawRectangle(rect, { 200, 200, 0, 150 });
		break;
		
	case CoinState::BRICK:
		leftTile = { coordinates.x - 1, coordinates.y };
		rightTile = { coordinates.x + 1, coordinates.y };

		for (list = editor->tiles.start; list != nullptr; list = list->next)
		{
			if (list->data->type == TileType::COIN)
			{
				if (list->data->coordinates == leftTile) left = true;
				if (list->data->coordinates == rightTile) right = true;

				if (left && right) break;
			}
		}

		if (left && !right) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &brickSprites[2]);
		else if (!left && right) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &brickSprites[1]);
		else if (!left && !right) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &brickSprites[0]);
		else if (left && right) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &brickSprites[3]);
		break;

	case CoinState::COLLECTED:
		if (debug) app->render->DrawRectangle(rect, { 200, 200, 0, 150 });
		break;
	}
}

void Coin::Restart()
{
	pSwitch = false;
	timer = 0;
	if (state == CoinState::BRICK)
	{
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

	state = CoinState::COIN;
}
