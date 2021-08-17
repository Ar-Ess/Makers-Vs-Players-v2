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

	for (int i = 0; i < 5; i++) idleAnim.PushBack({ 43 * i, 0, 43, 53 }); // IDLE
	idleAnim.loop = true;
	idleAnim.speed = 6;
	for (int i = 0; i < 7; i++) walkAnim.PushBack({ 43 * i, 53, 43, 51 }); // WALK
	walkAnim.loop = true;
	walkAnim.speed = 11;

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
	app->render->DrawTexture(texture, body->position.x, body->position.y - 6, 1, 1, &currAnim->GetCurrentFrame(), false, true, 0.0, (SDL_RendererFlip)((int)currAnim->mirror));

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
		currAnim = &walkAnim;
		currAnim->mirror = true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		currAnim = &idleAnim;
		currAnim->mirror = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) body->Move(5.0f, Direction::RIGHT, 300.0f);
		else body->Move(5.0f, Direction::RIGHT, 200.0f);
		currAnim = &walkAnim;
		currAnim->mirror = false;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		currAnim = &idleAnim;
		currAnim->mirror = false;
	}
}

void Player::UpdatePosition(iPoint pos)
{
	body->position.x = pos.x;
	body->position.y = pos.y;
	body->rect.x = pos.x;
	body->rect.y = pos.y;
}
