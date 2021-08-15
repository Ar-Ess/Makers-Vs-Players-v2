#ifndef __COIN_H__
#define __COIN_H__

#include "Tile.h"
#include "Point.h"

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

    Coin(fPoint pos, iPoint coords);

    virtual ~Coin();

private:

    CoinState state = CoinState::COIN;

};

#endif // __COIN_H__
