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
	texture = app->tex->Load("Assets/Textures/Items/item_coin.png");
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
	if (state == CoinState::COIN && utils.CheckCollision(rect, editor->player->body->ReturnBodyRect()))
	{
		state = CoinState::COLLECTED;
	}
}

void Coin::Draw(float dt)
{
	bool debug = editor->phys->debug;

	switch (state)
	{
	case CoinState::COIN:
		if (editor->GetState() == EditorState::PLAYING) coinAnim.Update(dt);
		else if (editor->GetState() == EditorState::EDITING) coinAnim.Reset();
		app->render->DrawTexture(texture, position.x, position.y, 1, 1, &coinAnim.GetCurrentFrame(), false);

		if (debug) app->render->DrawRectangle(rect, { 200, 200, 0, 150 });
		break;
		
	case CoinState::BLOCK:
		// textura de bloc
		if (debug) app->render->DrawRectangle({(int)position.x, (int)position.y, 53, 53}, { 200, 200, 0, 150 });
		break;

	case CoinState::COLLECTED:
		if (debug) app->render->DrawRectangle(rect, { 200, 200, 0, 150 });
		break;
	}
}

void Coin::Restart()
{
	state = CoinState::COIN;
}
