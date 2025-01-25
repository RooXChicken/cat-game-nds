#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include <../../entity/entity.h>

//forward declaration to prevent circular header files
class Entity;

class Scene
{
    private:
    public:
        Entity* entities[128];
        Vector2 camera_position;
        Vector2 old_camera_position;

        Scene();
        virtual void load();
        virtual Entity* spawn_entity(Entity* _entity);

        virtual void update();
        virtual void render();
        
        virtual void unload();
};

#endif