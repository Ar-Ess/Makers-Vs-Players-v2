#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "App.h"

#include "Entity.h"
#include "Animation.h"
#include "Physics.h"

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

    void RestartPlayer();

    void PlayerControls();

    SDL_Rect GetCollisionBounds()
    {
        SDL_Rect collision = {};
        body->GetCollision(collision);
        return collision;
    }

public: //Characteristics


public:
    SDL_Texture* texture = nullptr;
    DynamicBody* body = nullptr;

public:
    bool godMode = false;
};

#endif // __PLAYER_H__