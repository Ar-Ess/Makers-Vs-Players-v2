#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"

#include "Point.h"

class Item : public Entity
{
public:

    Item() : Entity(EntityType::ITEM) {};
    virtual ~Item() {};
};

#endif // __ITEM_H__
