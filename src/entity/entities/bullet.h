#ifndef BULLET_ENTITY_H_INCLUDED
#define BULLET_ENTITY_H_INCLUDED

#include <../../entity/entity.h>

class Bullet : public Entity
{
    private:
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
    _move();
}

void Bullet::_move()
{
    position += velocity;
    velocity = velocity * 0.8;

    if(velocity.length() < 0.1)
        destroy();
}

void Bullet::draw(Vector2 _camera)
{
    sprite.oam->position = position;
    sprite.draw(_camera);
}

#endif