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

void Player::Refill()
{
    health = maxHealth;
}

void Player::RestartPlayer()
{
}
