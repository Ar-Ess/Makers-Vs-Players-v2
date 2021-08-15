#include "Boss.h"
#include "App.h"
#include "Textures.h"

Boss::Boss() : Entity(EntityType::BOSS)
{
}

Boss::Boss(BossClass bClass) : Entity(EntityType::BOSS)
{
	bossClass = bClass;
	// LIFE -> ((x / 1.5) + 20)
	// STRG -> ((x / 2) + 8)
	// DEFS -> ((x / 4) + 2) nose
	// EXPR -> expActLvl + expNextLvl / 2
}

Boss::~Boss()
{
}

void Boss::SetUp(SDL_Rect combatCollider, int xexp, int xhealth, int xstrength, int xdefense)
{
	colliderCombat = combatCollider;
	exp = xexp;
	health = xhealth;
	maxHealth = xhealth;
	strength = xstrength;
	defense = xdefense;
}

void Boss::Refill()
{
	health = maxHealth;
}
