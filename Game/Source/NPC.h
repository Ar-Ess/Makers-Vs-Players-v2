#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"

#include "Point.h"
#include "SString.h"
#include "Animation.h"

#include "Collider.h"

#include "SDL/include/SDL.h"

class World;
enum Places;

enum NPCtype
{
    KNIGHT_M_1,
    KNIGHT_F,
    OLD,
    PRIEST,
    NO_NPC,
    BARMAN,
    WITCH,
    ADVENTURER,
    FINAL_BOSS,
    SHOP_WOMAN, 
    CAT,
    CITIZEN_1,
    CITIZEN_2,
    CITIZEN_3,
    CITIZEN_4,
    CITIZEN_5,
    CITIZEN_6,
    CITIZEN_7,
    CITIZEN_8,
    CITIZEN_9,
    CITIZEN_10,
    GARDENER,
    FORESTAL,
    KNIGHT_M_2,
    KNIGHT_M_3,
    KNIGHT_M_4,
};

class NPC : public Entity
{
public:

    NPC();

    NPC(SDL_Rect colliderx);

    ~NPC();

    void SetUp(iPoint position, NPCtype type, Places place, int dialog, int direction = 1);

    void Update(SDL_Rect pCollAmp);

    void Draw();

    void Delete();

    Places GetPlace() const
    {
        return place;
    }

private:
    friend class World;

    SDL_Rect rect;
    SDL_Rect contactCollider;
    NPCtype npcType;
    Places place;
    SDL_Texture* texture = nullptr;
    int dialog;
    int direction;

    Collision collisionUtils;

    bool active;
};

#endif // __NPC_H__
