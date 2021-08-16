#include "Player.h"

#include "Scene.h"
#include "Textures.h"

Player::Player() : Entity(EntityType::PLAYER)
{

}

bool Player::Start()
{
	UpdatePosition({300, 1000});
	texture = app->tex->Load("Assets/Textures/Entities/Player/spritesheet_player.png");

	for (int i = 0; i < 5; i++) idleAnim.PushBack({ 53 * i, 0, 53, 53 }); // IDLE
	idleAnim.loop = true;
	idleAnim.speed = 5;
	for (int i = 0; i < 7; i++) walkAnim.PushBack({ 53 * i, 0, 53, 53 }); // WALK
	walkAnim.loop = true;
	walkAnim.speed = 5;

	currAnim = &idleAnim;

	return true;
}

bool Player::Update(float dt)
{
	PlayerControls();
	//LOG("X: %d Y: %d", body->position.x, body->position.y);
    return true;
}

bool Player::Draw(float dt)
{
	currAnim->Update(dt);
	app->render->DrawTexture(texture, body->position.x, body->position.y, 1, 1, &currAnim->GetCurrentFrame(), false);

    return true;
}

void Player::SetTexture(SDL_Texture *tex)
{
    texture = tex;
}

void Player::RestartPlayer()
{
}

void Player::PlayerControls()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		body->Jump(-400.0f, false);
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) body->Move(5.0f, Direction::LEFT, 300.0f);
		else body->Move(5.0f, Direction::LEFT, 200.0f);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) body->Move(5.0f, Direction::RIGHT, 300.0f);
		else body->Move(5.0f, Direction::RIGHT, 200.0f);
	}
}

void Player::UpdatePosition(iPoint pos)
{
	body->position.x = pos.x;
	body->position.y = pos.y;
	body->rect.x = pos.x;
	body->rect.y = pos.y;
}
