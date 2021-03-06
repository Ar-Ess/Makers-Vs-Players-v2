#include "App.h"
#include "Render.h"
#include "Scene.h"
#include "LevelEditorScene.h"
#include "Textures.h"
#include "GroundTile.h"

GroundTile::GroundTile()
{

}

GroundTile::GroundTile(fPoint pos, iPoint coords, LevelEditor* lE, Z axis)
{
	switch (lE->GetBackground())
	{
	case Background::SKY: 
		texture = app->tex->Load("Assets/Textures/Tilesets/ground_sky_tileset.png"); 
		break;

	case Background::NOON:
		texture = app->tex->Load("Assets/Textures/Tilesets/ground_noon_tileset.png");
		break;
	}

	position = pos;
	coordinates = coords;
	editor = lE;
	zAxis = axis;
	type = TileType::GROUND;
	body = (StaticBody*)editor->phys->CreateBody(BodyType::STATIC_BODY, pos, { (int)pos.x, (int)pos.y, TILE_SIZE, TILE_SIZE });
}

GroundTile::~GroundTile()
{
	delete body;
}

void GroundTile::Start()
{
}

void GroundTile::Update(float dt)
{
	return;
}

void GroundTile::Draw(float dt)
{
	iPoint upTile = { coordinates.x, coordinates.y - 1 };
	iPoint downTile = { coordinates.x, coordinates.y + 1 };
	iPoint leftTile = { coordinates.x - 1, coordinates.y };
	iPoint rightTile = { coordinates.x + 1, coordinates.y };
	iPoint upLTile = { coordinates.x - 1, coordinates.y - 1 };
	iPoint downLTile = { coordinates.x - 1, coordinates.y + 1 };
	iPoint upRTile = { coordinates.x + 1, coordinates.y - 1 };
	iPoint downRTile = { coordinates.x + 1, coordinates.y + 1 };

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool upL = false;
	bool downL = false;
	bool upR = false;
	bool downR = false;

	ListItem<Tile*>* list;
	for (list = editor->tiles.start; list != nullptr; list = list->next)
	{
		if (list->data->type == TileType::GROUND)
		{
			if (list->data->coordinates == upTile) up = true;
			if (list->data->coordinates == downTile) down = true;
			if (list->data->coordinates == leftTile) left = true;
			if (list->data->coordinates == rightTile) right = true;
			if (list->data->coordinates == upLTile) upL = true;
			if (list->data->coordinates == downLTile) downL = true;
			if (list->data->coordinates == upRTile) upR = true;
			if (list->data->coordinates == downRTile) downR = true;

			if (up && down && left && right && upL && downL && upR && downR) break;
		}
	}

	if (!up && left && !down && !right) //No Diagonal
	{
		app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[3]);
	}
	else if (up && !left && !down && !right) //No Diagonal
	{
		app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[3], true, true, 90);
	}
	else if (!up && !left && down && !right) //No Diagonal
	{
		app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[3], true, true, -90);
	}
	else if (!up && !left && !down && right) //No Diagonal
	{
		app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[3], true, true, 180);
	}
	else if (!up && left && down && !right) //Diagonal DownL
	{
		if (downL) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[2]);
		else if (!downL) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[11], true, true, -90);
	}
	else if (up && left && !down && !right) //Diagonal UpL
	{
		if (upL) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[2], true, true, 90);
		else if (!upL) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[11]);
	}
	else if (up && !left && !down && right) //Diagonal UpR
	{
		if (upR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[2], true, true, 180);
		else if (!upR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[11], true, true, 90);
	}
	else if (!up && !left && down && right) //Diagonal DownR
	{
		if (downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[2], true, true, -90);
		else if (!downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[11], true, true, 180);
	}
	else if (!up && left && down && right) //Diagonal DownL, DownR, Both
	{
		if (downL && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[0]);
		else if (downL && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[8], true, true, 180);
		else if (!downL && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[9], true, true, 180);
		else if (!downL && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[10], true, true, 180);
	}
	else if (up && !left && down && right) //Diagonal UpR, DownR, Both
	{
		if (upR && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[0], true, true, -90);
		else if (upR && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[9], true, true, 90);
		else if (!upR && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[8], true, true, 90);
		else if (!upR && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[10], true, true, 90);
	}
	else if (up && left && !down && right) //Diagonal UpL, UpR, Both
	{
		if (upL && upR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[0], true, true, 180);
		else if (upL && !upR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[9]);
		else if (!upL && upR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[8]);
		else if (!upL && !upR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[10]);
	}
	else if (up && left && down && !right) //Diagonal UpL, DownL, Both
	{
		if (upL && downL) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[0], true, true, 90);
		else if (upL && !downL) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[8], true, true, -90);
		else if (!upL && downL) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[9], true, true, -90);
		else if (!upL && !downL) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[10], true, true, -90);
	}
	else if (up & left & down & right)
	{
		if (upL && upR && downL && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[1]);
		else if (!upL && upR && downL && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[6]);
		else if (upL && !upR && downL && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[6], true, true, 90);
		else if (upL && upR && !downL && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[6], true, true, -90);
		else if (upL && upR && downL && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[6], true, true, 180);
		else if (!upL && !upR && !downL && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[12]);
		else if (upL && !upR && !downL && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[12], true, true, 180);
		else if (!upL && upR && !downL && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[12], true, true, -90);
		else if (!upL && !upR && downL && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[12], true, true, 90);
		else if (upL && !upR && !downL && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[14], true, true, 90);
		else if (!upL && upR && downL && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[14]);
		else if (!upL && !upR && downL && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[7]);
		else if (upL && upR && !downL && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[7], true, true, 180);
		else if (!upL && upR && !downL && downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[7], true, true, -90);
		else if (upL && !upR && downL && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[7], true, true, 90);
		else if (!upL && !upR && !downL && !downR) app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[13]);

	}
	else if (!up & left & !down & right)
	{
		app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[5]);
	}
	else if (up & !left & down & !right)
	{
		app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[5], true, true, 90);
	}
	else
	{
		app->render->DrawTexture(texture, position.x, position.y, 1, 1, &sprites[4]);
	}
}

void GroundTile::Restart()
{
	return;
}
