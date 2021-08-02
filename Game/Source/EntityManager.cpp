#include "App.h"
#include "EntityManager.h"

#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Item.h"
#include "NPC.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{

}

EntityManager::~EntityManager()
{}

bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	return ret;
}

bool EntityManager::CleanUp()
{
	if (!active) return true;

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* ret = nullptr;

	switch (type)
	{
		case EntityType::PLAYER: 
			ret = new Player(); 
			players.Add((Player*)ret);
			break;
		case EntityType::NPC: 
			ret = new NPC();
			NPCs.Add((NPC*)ret);
			break;
		case EntityType::ENEMY: 
			ret = new Enemy();
			enemies.Add((Enemy*)ret);
			break;
		case EntityType::BOSS:
			ret = new Boss();
			bosses.Add((Boss*)ret);
			break;
		case EntityType::ITEM: 
			ret = new Item();  
			items.Add((Item*)ret);
			break;
		default: 
			break;
	}

	return ret;
}

bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}
