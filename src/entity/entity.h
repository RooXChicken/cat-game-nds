#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <../math/vector2.h>
#include <../../src/assets/sprite.h>

enum EntityType
{
    EMPTY = 0,
    PLAYER = 1
};

class Entity
{
    public:
        EntityType type;
        Vector2 position;
        Sprite sprite;

        void spawn(Vector2 _position, Sprite _sprite);
        void draw(Vector2 _camera_position);
};

#endif