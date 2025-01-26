#ifndef PLAYER_ENTITY_H_INCLUDED
#define PLAYER_ENTITY_H_INCLUDED

#include <../../entity/entity.h>
#include <../../entity/entities/bullet.h>

class Player : public Entity
{
    private:
        Sprite body_idle;
        Sprite body_walk;

        Sprite arms_idle;
        Sprite arms_walk;

        Sprite arms;

        Sprite crosshair;
        Vector2 crosshair_raw_position;

        bool flip;
        double frame = 0;

        double move_speed = 0.5;
        double decel_speed = 0.2;
        double speed_cap = 2;

        Vector2 velocity = {0, 0};
        Vector2 move_dir = {0, 0};

        Vector2 aim = {0, 0};
        double weapon_rotation = 0;

        void _move();
        Sprite weapon;
        int shoot_cooldown = 0;

    public:
        Vector2 center;
        
        void spawn(Scene* _scene, int _id) override;
        void update(Vector2 _camera) override;
        void draw(Vector2 _camera) override;
};

void Player::spawn(Scene* _scene, int _id)
{
    type = EntityType::PLAYER;
    scene = _scene;
    id = _id;

    arms_idle = Sprite(SpriteType::BELLA_IDLE_ARMS, -1, -1);
    arms_walk = Sprite(SpriteType::BELLA_WALK_ARMS, -1, 0);

    body_idle = Sprite(SpriteType::BELLA_IDLE, -1, 0);
    body_walk = Sprite(SpriteType::BELLA_WALK, -1, 0);

    crosshair = Sprite(SpriteType::CROSSHAIR, -1, 0);
    center = {12, 16};

    weapon = Sprite(SpriteType::TREAT_PISTOL, -1, -1);
    weapon.oam->make_affine(-1);
    weapon.oam->priority = 0;

    crosshair.oam->priority = 0;

    sprite = body_idle;
    arms = arms_idle;
}

void Player::update(Vector2 _camera)
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

    touchPosition touch;
    touchRead(&touch);

    if(keysHeld() & KEY_TOUCH)
        crosshair_raw_position = Vector2{(double)touch.px, (double)touch.py} - Vector2{3.5, 3.5};

    aim = (position - _camera + center - crosshair_raw_position).normalize();

    if(shoot_cooldown > 0)
        shoot_cooldown--;

    if((keysHeld() & KEY_TOUCH) && shoot_cooldown <= 0)
    {
        Bullet* _bullet = (Bullet*)scene->spawn_entity(new Bullet());
        _bullet->position = position + center;
        _bullet->velocity = aim * -3;

        shoot_cooldown = 20;
    }

    if(abs(velocity.length()) < 0.1)
    {
        sprite.oam->pointer = body_idle.oam->pointer;
        arms.oam->pointer = arms_idle.oam->pointer;

        sprite.data = body_idle.data;
        arms.data = arms_idle.data;

        frame = 0;
    }
    else
    {
        sprite.oam->pointer = body_walk.oam->pointer;
        arms.oam->pointer = arms_walk.oam->pointer;

        sprite.data = body_walk.data;
        arms.data = arms_walk.data;

        if((!flip && velocity.x > 0) || (flip && velocity.x < 0))
            frame += velocity.length()/12;
        else
            frame -= velocity.length()/12;
    }

    _move();
}

void Player::_move()
{
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
    if(keysHeld() & KEY_TOUCH)
    {
        double _flip_when = (position.x - _camera.x) + center.x;
        if(crosshair_raw_position.x < _flip_when)
            flip = true;
        else if(crosshair_raw_position.x > _flip_when)
            flip = false;

        weapon_rotation = (atan2(aim.x, aim.y)) * -60 + (90 * (flip ? -1.0 : 1.0));
    }
    else
    {
        if(velocity.x < 0)
            flip = true;
        else if(velocity.x > 0)
            flip = false;

        weapon_rotation = 180;
    }

    sprite.oam->position = position;
    arms.oam->position = position;

    sprite.oam->flip_h = flip;
    arms.oam->flip_h = flip;

    sprite.frame = frame;
    arms.frame = frame;

    weapon.oam->position = position;
    weapon.oam->position.y += center.y;

    weapon.draw_affine(_camera, weapon_rotation, {(flip ? 1.0 : -1.0), -1.0});

    sprite.draw(_camera);
    arms.draw(_camera);

    crosshair.oam->position = crosshair_raw_position;
    crosshair.draw({0, 0});
}

#endif