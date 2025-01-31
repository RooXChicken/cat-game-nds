#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <../math/vector2.h>
#include <../../src/assets/sprite.h>
#include <../../src/scene/scene.h>

struct EntityTypes
{
    enum class Type
    {
        EMPTY = 0,
        PLAYER = 1,
        BULLET = 2,
        ITEM_ENTITY = 3
    };
};

//forward declaration to prevent circular header files
class Scene;

class Entity
{
    protected:
    public:
        Scene* scene = nullptr;
        EntityTypes::Type type = EntityTypes::Type::EMPTY;
        int id = -1;

        Vector2 position = {0, 0};
        Vector2 velocity = {0, 0};
        Sprite sprite;

        Entity();
        virtual void spawn(Scene* _scene, int _id);
        virtual void update(Vector2 _camera);
        virtual void draw(Vector2 _camera);
        virtual void destroy();
};

#endif