#include <../entities/bullet.h>

Bullet::Bullet(Sprite _sprite, double _damage)
{
    sprite = _sprite;
    damage = _damage;
}

void Bullet::spawn(Scene* _scene, int _id)
{
    type = EntityTypes::Type::BULLET;
    scene = _scene;
    id = _id;
}

void Bullet::update(Vector2 _camera)
{
    position += velocity;
    if(velocity.length() < 0.1)
        destroy();
    else
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
    sprite.oam.position = position;
    sprite.draw(_camera);
}