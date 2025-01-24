#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <../../entity/entity.h>

class Player : public Entity
{
    private:
        Sprite body_idle;
        Sprite body_walk;

        Sprite arms_idle;
        Sprite arms_walk;

        Sprite arms;

        bool flip;
        double frame = 0;

        double move_speed = 0.5;
        double decel_speed = 0.2;
        double speed_cap = 2;

        Vector2 velocity = {0, 0};
        Vector2 move_dir = {0, 0};

    public:
        // Player(Vector2 _position);
        void spawn() override;
        void update() override;
        void draw(Vector2 _camera) override;
};

void Player::spawn()
{
    type = EntityType::PLAYER;

    body_idle = Sprite(SpriteType::BELLA_IDLE, -1, -1);
    body_walk = Sprite(SpriteType::BELLA_WALK, -1, 0);

    arms_idle = Sprite(SpriteType::BELLA_IDLE_ARMS, -1, 0);
    arms_walk = Sprite(SpriteType::BELLA_WALK_ARMS, -1, 0);

    sprite = body_idle;
    arms = arms_idle;
    
}

void Player::update()
{
    move_dir = {0, 0};
    if(keysHeld() & KEY_LEFT)
        move_dir.x -= 1;
    if(keysHeld() & KEY_RIGHT)
        move_dir.x += 1;
    if(keysHeld() & KEY_UP)
        move_dir.y -= 1;
    if(keysHeld() & KEY_DOWN)
        move_dir.y += 1;

    move_dir = move_dir.normalize();
    velocity += move_dir * move_speed;

    position += velocity;

    if(abs(velocity.length()) < 0.1)
    {
        sprite.pointer = body_idle.pointer;
        arms.pointer = arms_idle.pointer;

        sprite.data = body_idle.data;
        arms.data = arms_idle.data;

        frame = 0;
    }
    else
    {
        sprite.pointer = body_walk.pointer;
        arms.pointer = arms_walk.pointer;

        sprite.data = body_walk.data;
        arms.data = arms_walk.data;

        if(velocity.x < 0)
            flip = true;
        else if(velocity.x > 0)
            flip = false;

        frame += velocity.length()/12;
    }

    if(velocity.x > 0)
    {
        velocity.x -= decel_speed;

        if(velocity.x > speed_cap)
            velocity.x -= move_speed;

        if(velocity.x < 0)
            velocity.x = 0;
    }
    else if(velocity.x < 0)
    {
        velocity.x += decel_speed;

        if(velocity.x < -speed_cap)
            velocity.x += move_speed;

        if(velocity.x > 0)
            velocity.x = 0;
    }

    if(velocity.y > 0)
    {
        velocity.y -= decel_speed;

        if(velocity.y > speed_cap)
            velocity.y -= move_speed;

        if(velocity.y < 0)
            velocity.y = 0;
    }
    else if(velocity.y < 0)
    {
        velocity.y += decel_speed;

        if(velocity.y < -speed_cap)
            velocity.y += move_speed;

        if(velocity.y > 0)
            velocity.y = 0;
    }
}

void Player::draw(Vector2 _camera)
{
    sprite.priority = 1;

    sprite.position = position;
    arms.position = position;

    sprite.flip_h = flip;
    arms.flip_h = flip;

    sprite.frame = frame;
    arms.frame = frame;

    sprite.draw(_camera);
    arms.draw(_camera);
}

#endif