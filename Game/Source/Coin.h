#ifndef __COIN_H__
#define __COIN_H__

#include "Tile.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Rect;

enum class CoinState
{
    COIN,
    BLOCK,
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

};

#endif // __COIN_H__
