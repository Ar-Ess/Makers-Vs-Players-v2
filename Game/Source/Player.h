#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "App.h"

#include "Entity.h"
#include "Animation.h"

#include "Point.h"
#include "Log.h"

#include "SDL/include/SDL.h"

class Animation;

class Player : public Entity
{
public:

    Player();

    bool Update(float dt);

    bool Draw();

    void SetTexture(SDL_Texture *tex);

    void Refill();

    void RestartPlayer();

    SDL_Rect GetCollisionBounds()
    {
        return rect;
    }

public:

    int health = 0;
    int maxHealth = 0;

    int healthStat = 1;
    int defenseStat = 1;
    int strengthStat = 1;
    int velocityStat = 1;
    
    int lvl = 1;
    int exp = 0;

    int playerSpeed = 8;

public:
    SDL_Texture* texture;

    SDL_Rect rect;
    SDL_Rect wCollider;

public:
    bool godMode = false;
};

#endif // __PLAYER_H__
