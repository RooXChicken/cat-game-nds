#ifndef BULLET_ENTITY_H_INCLUDED
#define BULLET_ENTITY_H_INCLUDED

#include <../../entity/entity.h>

class Bullet : public Entity
{
    private:
        void _move();

    public:
        Vector2 velocity = {0, 0};
        double decel_speed = 0.04;
        double damage = 0;

        Bullet(Sprite _sprite, double _damage);
        void spawn(Scene* _scene, int _id) override;
        void update(Vector2 _camera) override;
        void draw(Vector2 _camera) override;
};

#endif