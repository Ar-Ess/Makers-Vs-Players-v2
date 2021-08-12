#include "Player.h"

#include "Scene.h"

Player::Player() : Entity(EntityType::PLAYER)
{

}

bool Player::Start()
{
	UpdatePosition({300, 1000});

	return true;
}

bool Player::Update(float dt)
{
	PlayerControls();
	//LOG("X: %d Y: %d", body->position.x, body->position.y);
    return true;
}

bool Player::Draw()
{
    return false;
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
