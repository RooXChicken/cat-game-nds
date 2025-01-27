#ifndef BULLET_ENTITY_H_INCLUDED
#define BULLET_ENTITY_H_INCLUDED

#include <../../entity/entity.h>

class Bullet : public Entity
{
    private:
        double decel_speed = 0.04;
        void _move();

    public:
        Vector2 velocity = {0, 0};
        void spawn(Scene* _scene, int _id) override;
        void update(Vector2 _camera) override;
        void draw(Vector2 _camera) override;
};

#endif