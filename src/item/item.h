#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <../../src/assets/sprite.h>
#include <../../src/entity/entities/player.h>

// enum ItemType
// {
//     EMPTY = 0,
//     TREAT_PISTOL = 1
// };

class Player;

class Item
{
    public:
        // ItemType type;
        Player* player = nullptr;
        Vector2 aim = {0, 0};
        Sprite sprite;

        Item();
        virtual void spawn(Player* _player);
        virtual void use();
        virtual void update();
        virtual void draw(Vector2 _camera);
};

#endif