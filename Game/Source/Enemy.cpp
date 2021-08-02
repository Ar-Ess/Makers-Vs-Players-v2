#define _CRT_SECURE_NO_WARNINGS

#include "App.h"
#include "Scene.h"

#include "Enemy.h"

#include "Log.h"

#define DEFAULT_PATH_LENGTH 50

Enemy::Enemy() : Entity(EntityType::ENEMY)
{

}

Enemy::~Enemy()
{

}

void Enemy::Refill()
{
    health = maxHealth;
}
