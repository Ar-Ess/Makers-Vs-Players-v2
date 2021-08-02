#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"

#include "List.h"
enum EnemyClass;
enum BossClass;

class EntityManager : public Module
{
public:

	EntityManager();

	virtual ~EntityManager();

	bool Awake(pugi::xml_node&);

	bool Update(float dt);

	bool CleanUp();

	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);

public:

	List<Player*> players;
	List<Enemy*> enemies;
	List<Boss*> bosses;
	List<NPC*> NPCs;
	List<Item*> items;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
};

#endif // __ENTITYMANAGER_H__
