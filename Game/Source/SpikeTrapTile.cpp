#include "App.h"
#include "Render.h"
#include "Scene.h"
#include "LevelEditorScene.h"
#include "Textures.h"
#include "SpikeTrapTile.h"
#include "Player.h"

SpikeTrapTile::SpikeTrapTile()
{

}

SpikeTrapTile::SpikeTrapTile(fPoint pos, iPoint coords, LevelEditor* lE, Z axis)
{
	switch (lE->GetBackground())
	{
	case Background::SKY:
		texture = app->tex->Load("Assets/Textures/Tilesets/spike_trap_sky_tileset.png");
		break;

	case Background::NOON:
		texture = app->tex->Load("Assets/Textures/Tilesets/spike_trap_sky_tileset.png");
		break;
	}

	position = pos;
	coordinates = coords;
	editor = lE;
	zAxis = axis;
	type = TileType::SPIKE_TRAP;
	body = (StaticBody*)editor->phys->CreateBody(BodyType::STATIC_BODY, pos, { (int)pos.x, (int)pos.y, TILE_SIZE, TILE_SIZE });
}

SpikeTrapTile::~SpikeTrapTile()
{
	delete body;
}

void SpikeTrapTile::Update(float dt)
{
	if (editor->phys->ReturnPlayerCollision() == body)
	{
		editor->PreviewDeath();
	}

	return;
}

void SpikeTrapTile::Draw(float dt)
{
	app->render->DrawTexture(texture, position.x, position.y, 1, 1, false);
}

void SpikeTrapTile::Restart()
{
}
