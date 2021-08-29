#include "App.h"
#include "Render.h"
#include "Scene.h"
#include "LevelEditorScene.h"
#include "Textures.h"
#include "ArrowSignTile.h"

ArrowSignTile::ArrowSignTile()
{

}

ArrowSignTile::ArrowSignTile(fPoint pos, iPoint coords, LevelEditor* lE, Z axis, int angl)
{
	switch (lE->GetBackground())
	{
	case Background::SKY:
		texture = app->tex->Load("Assets/Textures/Tilesets/arrow_sign_sky_tileset.png");
		break;

	case Background::NOON:
		texture = app->tex->Load("Assets/Textures/Tilesets/arrow_sign_sky_tileset.png");
		break;
	}

	position = pos;
	coordinates = coords;
	editor = lE;
	zAxis = axis;
	angle = angl;
	type = TileType::ARROW_SIGN;
	fourByFour = true;
	rect = {(int)pos.x, (int)pos.y, TILE_SIZE * 2, TILE_SIZE * 2};
}

ArrowSignTile::~ArrowSignTile()
{
	return;
}

void ArrowSignTile::Update(float dt)
{
	return;
}

void ArrowSignTile::Draw(float dt)
{
	bool debug = editor->phys->debug;

	switch (angle)
	{
	case 0: app->render->DrawTexture(texture, position.x, position.y, 1, 1); break;
	case 1: app->render->DrawTexture(texture, position.x, position.y, 1, 1, (SDL_Rect*)0, false, true, 45); break;
	case 2: app->render->DrawTexture(texture, position.x, position.y, 1, 1, (SDL_Rect*)0, false, true, 90); break;
	case 3: app->render->DrawTexture(texture, position.x, position.y, 1, 1, (SDL_Rect*)0, false, true, -45, SDL_FLIP_HORIZONTAL); break;
	case 4: app->render->DrawTexture(texture, position.x, position.y, 1, 1, (SDL_Rect*)0, false, true, 0, SDL_FLIP_HORIZONTAL); break;
	case 5: app->render->DrawTexture(texture, position.x, position.y, 1, 1, (SDL_Rect*)0, false, true, 45, SDL_FLIP_HORIZONTAL); break;
	case 6: app->render->DrawTexture(texture, position.x, position.y, 1, 1, (SDL_Rect*)0, false, true, -90); break;
	case 7: app->render->DrawTexture(texture, position.x, position.y, 1, 1, (SDL_Rect*)0, false, true, -45); break;
	}

	if (debug) app->render->DrawRectangle(rect, { 0, 200, 200, 150 });

	return;
}

void ArrowSignTile::Restart()
{
	return;
}
