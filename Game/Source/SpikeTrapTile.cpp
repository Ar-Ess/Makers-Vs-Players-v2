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

SpikeTrapTile::SpikeTrapTile(fPoint pos, iPoint coords, LevelEditor* lE)
{
	switch (lE->GetBackground())
	{
	case Background::SKY:
		//texture = app->tex->Load("Assets/Textures/Tilesets/ground_sky_tileset.png");
		break;

	case Background::NOON:
		//texture = app->tex->Load("Assets/Textures/Tilesets/ground_noon_tileset.png");
		break;
	}

	position = pos;
	coordinates = coords;
	editor = lE;
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
}

void SpikeTrapTile::Restart()
{
}
