#include "Player.h"

#include "Scene.h"

Player::Player() : Entity(EntityType::PLAYER)
{

}

bool Player::Update(float dt)
{
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
		body->Jump(-300.0f, false);
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
