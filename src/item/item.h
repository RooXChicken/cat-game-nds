#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <../../src/assets/sprite.h>
#include <../../src/entity/entities/player.h>
#include <../../src/assets/sound.h>

struct ItemTypes
{
    enum Type
    {
        EMPTY = 0,
        TREAT_PISTOL = 1,
        QTIP_RIFLE = 2
    };
};

class Player;

class Item
{
    protected:
        Sound* use_sound;
    public:
        ItemTypes::Type type;
        Player* player = nullptr;
        Vector2 aim = {0, 0};
        Sprite sprite;

        Item(ItemTypes::Type);
        virtual void spawn(Player* _player);
        
        virtual void use();
        virtual bool can_swap() { return true; }

        virtual void update();
        virtual void draw(Vector2 _camera);

        virtual void destroy();
};

#endif