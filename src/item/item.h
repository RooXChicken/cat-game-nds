#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <../math/vector2.h>
#include <../../src/assets/sprite.h>

enum ItemType
{
    EMPTY = 0
};

class Item
{
    public:
        ItemType type;
        Sprite sprite;

        Item();
        virtual void spawn();
        virtual void use();
        virtual void update();
        virtual void draw(Vector2 _camera);
};

#endif