#include "App.h"
#include "Render.h"
#include "Scene.h"
#include "Animation.h"
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

	for (int i = 0; i < 4; i++)
	{
		if (i != 3) spikeAnim.PushBack({ 53 * i, 0, 53, 53 });
		else spikeAnim.PushBack({ 53, 0, 53, 53 });
	}
	spikeAnim.loop = true;
	spikeAnim.speed = 8.0f;
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
	bool debug = editor->phys->debug;

	if (editor->GetState() == EditorState::PLAYING) spikeAnim.Update(dt);
	else if (editor->GetState() == EditorState::EDITING) spikeAnim.Reset();
	app->render->DrawTexture(texture, position.x, position.y, 1, 1, &spikeAnim.GetCurrentFrame(), false);

	if (debug) app->render->DrawRectangle({(int)position.x, (int)position.y, 53, 53}, { 200, 200, 0, 150 });
}

void SpikeTrapTile::Restart()
{
}
