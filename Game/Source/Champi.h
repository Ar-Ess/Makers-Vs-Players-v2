#ifndef __CHAMPI_H__
#define __CHAMPI_H__

#include "Tile.h"
#include "Animation.h"
#include "Point.h"
#include "Physics.h"

struct SDL_Rect;

enum class ChampiState
{
    CHAMPI,
    COLLECTED
};

class Champi : public Tile
{
public:

    Champi();

    Champi(fPoint pos, iPoint coords, LevelEditor* lE, Z axis);

    virtual ~Champi();

    void Start();

    void Update(float dt);

    void Draw(float dt);

    void Restart();

    Body* GetBody()
    {
        return (Body*)body;
    }

private:
    ChampiState state = ChampiState::CHAMPI;
    Animation champiAnim;
    SDL_Rect rect = {};
    DynamicBody* body = nullptr;
    Direction dir = Direction::LEFT;

    SDL_Rect champiSprites[4] = { {0, 0, 53, 53}, {53, 0, 53, 53}, {106, 0, 53, 53}, {159, 0, 53, 53} };
};

#endif // __CHAMPI_H__

