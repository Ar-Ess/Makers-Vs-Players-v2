#ifndef __ENTITY_H__
#define __ENTITY_H__

class Enemy;
class Player;
class Boss;
class Item;
class NPC;
struct SDL_Texture;

enum class EntityType
{
    PLAYER,
    ENEMY,
	BOSS,
    ITEM,
    NPC,
    MAP,
    UNKNOWN
};

class Entity
{
public:

    Entity(EntityType type) : type(type) {}

    virtual bool Update(float dt)
    {
        return true;
    }

public:
    EntityType type;
    SDL_Texture* texture = nullptr;
};

#endif // __ENTITY_H__