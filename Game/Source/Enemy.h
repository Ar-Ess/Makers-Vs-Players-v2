#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Animation.h"
#include "Render.h"
#include "Textures.h"
#include "Collider.h"

#include "DynArray.h"
#include "Point.h"

#include "SDL/include/SDL.h"

struct SDL_Rect;
struct Animation;
struct SDL_Texture;
struct CircleCollider;
struct GuiString;

class Enemy : public Entity
{
public:
    Enemy();

    virtual ~Enemy();

    void Refill();

public:

    int health;
    int maxHealth;
    int defense;
    int strength;
    int velocity;
    int lvl;
    int exp;

    GuiString* lvlText = nullptr;

public:

    Animation awakeAnim;
    Animation idleAnim;
    Animation attackAnim;
    Animation moveAnim;
    Animation bulletMantis;

    bool active;
};

#endif // __ENEMY_H__
