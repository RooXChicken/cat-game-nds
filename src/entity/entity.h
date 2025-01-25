#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <../math/vector2.h>
#include <../../src/assets/sprite.h>
#include <../../src/scene/scene.h>

enum EntityType
{
    EMPTY = 0,
    PLAYER = 1,
    BULLET = 2
};

//forward declaration to prevent circular header files
class Scene;

class Entity
{
    protected:
    public:
        Scene* scene = nullptr;
        EntityType type;
        int id = -1;

        Vector2 position;
        Sprite sprite;

        Entity();
        virtual void spawn(Scene* _scene, int _id);
        virtual void update(Vector2 _camera);
        virtual void draw(Vector2 _camera);
        virtual void destroy();
};

#endif