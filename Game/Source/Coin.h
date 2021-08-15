#ifndef __COIN_H__
#define __COIN_H__

#include "Tile.h"
#include "Animation.h"
#include "Point.h"

#define P_SWITCH_TIME 400

struct SDL_Rect;

enum class CoinState
{
    COIN,
    BRICK,
    COLLECTED
};

class Coin : public Tile
{
public:

    Coin();

    Coin(fPoint pos, iPoint coords, LevelEditor* lE);

    virtual ~Coin();

    void Update(float dt);

    void Draw(float dt);

    void Restart();

private:
    CoinState state = CoinState::COIN;
    Animation coinAnim;
    SDL_Rect rect = {};
    StaticBody* body = nullptr;

    unsigned short int timer = 0;
    bool pSwitch = false;

    SDL_Rect brickSprites[4] = { {0, 0, 53, 53}, {53, 0, 53, 53}, {106, 0, 53, 53}, {159, 0, 53, 53} };
};

#endif // __COIN_H__
