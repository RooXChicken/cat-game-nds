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

void Bullet::spawn(Scene* _scene, int _id)
{
    type = EntityType::BULLET;
    scene = _scene;
    id = _id;
    
    sprite = Sprite(SpriteType::CAT_TREAT, -1, 1);
}

void Bullet::update(Vector2 _camera)
{
    position += velocity;
    if(velocity.length() < 0.1)
        destroy();
    
    _move();
}

void Bullet::_move()
{
    if(velocity.x > 0)
    {
        velocity.x -= decel_speed;

        if(velocity.x < 0)
            velocity.x = 0;
    }
    else if(velocity.x < 0)
    {
        velocity.x += decel_speed;

        if(velocity.x > 0)
            velocity.x = 0;
    }

    if(velocity.y > 0)
    {
        velocity.y -= decel_speed;

        if(velocity.y < 0)
            velocity.y = 0;
    }
    else if(velocity.y < 0)
    {
        velocity.y += decel_speed;

        if(velocity.y > 0)
            velocity.y = 0;
    }
}

void Bullet::draw(Vector2 _camera)
{
    sprite.oam->position = position;
    sprite.draw(_camera);
}

#endif