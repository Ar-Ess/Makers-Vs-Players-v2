#include "Coin.h"

Coin::Coin()
{
}

Coin::Coin(fPoint pos, iPoint coords)
{
	position = pos;
	coordinates = coords;
	type = TileType::COIN;
	state = CoinState::COIN;
}

Coin::~Coin()
{
}
