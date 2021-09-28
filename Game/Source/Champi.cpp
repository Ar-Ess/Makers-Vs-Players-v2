#include "Champi.h"
#include "LevelEditorScene.h"

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

	body = (DynamicBody*)editor->phys->CreateBody(BodyType::DYNAMIC_BODY, pos, { (int)pos.x, (int)pos.y, TILE_SIZE, TILE_SIZE });
	body->SetCollidable(false);

	rect = { (int)pos.x + 12, (int)pos.y + 6, 53 - 24, 53 - 12 };
	zAxis = axis;

	/*for (int i = 0; i < 9; i++) champiAnim.PushBack({ 53 * i, 0, 53, 53 });
	champiAnim.loop = true;
	champiAnim.speed = 12.0f;*/
}

Champi::~Champi()
{
}

void Champi::Update(float dt)
{
	body->Move(5.0f, Direction::RIGHT, 5.0f);
}

void Champi::Draw(float dt)
{
}

void Champi::Restart()
{
}
