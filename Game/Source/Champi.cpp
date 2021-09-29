#include "Champi.h"
#include "LevelEditorScene.h"
#include "Player.h"

Champi::Champi()
{
}

Champi::Champi(fPoint pos, iPoint coords, LevelEditor* lE, Z axis)
{
	position = pos;
	coordinates = coords;
	type = TileType::CHAMPI;
	state = ChampiState::CHAMPI;
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

	body = (DynamicBody*)editor->phys->CreateBody(BodyType::DYNAMIC_BODY, pos, { (int)pos.x, (int)pos.y, TILE_SIZE, TILE_SIZE }, { 0, 0 }, { 0, 0 }, 1.0f);
	//body->SetCollidable(false);

	rect = { (int)pos.x, (int)pos.y, 53, 53 };
	zAxis = axis;

	/*for (int i = 0; i < 9; i++) champiAnim.PushBack({ 53 * i, 0, 53, 53 });
	champiAnim.loop = true;
	champiAnim.speed = 12.0f;*/
}

Champi::~Champi()
{
}

void Champi::Start()
{
	if (((editor->player->body->GetPosition().x + (PLAYER_W / 2)) - (body->GetPosition().x + 26)) >= 0) dir = Direction::LEFT;
	else dir = Direction::RIGHT;
}

void Champi::Update(float dt)
{
	switch (state)
	{
	case ChampiState::CHAMPI:
		body->Move(50.0f, dir, 50.0f);
		if (editor->phys->ReturnPlayerCollision() == body)
		{
			state = ChampiState::COLLECTED;
			body->SetCollidable(false);
		}
		break;

	case ChampiState::COLLECTED:
		break;
	}
}

void Champi::Draw(float dt)
{
	bool debug = editor->phys->debug;

	switch (state)
	{
	case ChampiState::CHAMPI:
		break;

	case ChampiState::COLLECTED:
		break;
	}
}

void Champi::Restart()
{
	body->SetPosition(fPoint{float(coordinates.x * 53), float(coordinates.y * 53)});
	body->SetCollidable(true);
}
