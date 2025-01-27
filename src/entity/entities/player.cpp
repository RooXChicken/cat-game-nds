#include <../entities/player.h>
#include <../entities/bullet.h>
#include <../../item/items/weapon.h>

void Player::spawn(Scene* _scene, int _id)
{
    type = EntityType::PLAYER;
    scene = _scene;
    id = _id;

    arms_idle = Sprite(SpriteType::BELLA_IDLE_ARMS, 126, -1);
    arms_walk = Sprite(SpriteType::BELLA_WALK_ARMS, 126, 0);

    body_idle = Sprite(SpriteType::BELLA_IDLE, 127, 0);
    body_walk = Sprite(SpriteType::BELLA_WALK, 127, 0);

    shoot_d = Sprite(SpriteType::BELLA_SHOOT_D, 126, 0);
    shoot_d2 = Sprite(SpriteType::BELLA_SHOOT_D2, 126, 0);
    shoot_m = Sprite(SpriteType::BELLA_SHOOT_M, 126, 0);
    shoot_m2 = Sprite(SpriteType::BELLA_SHOOT_M2, 126, 0);
    shoot_u = Sprite(SpriteType::BELLA_SHOOT_U, 126, 0);
    shoot_u2 = Sprite(SpriteType::BELLA_SHOOT_U2, 126, 0);

    crosshair = Sprite(SpriteType::CROSSHAIR, 1, 0);
    center = {12, 16};

    inventory[0] = (Item*)new Weapon();
    inventory[0]->spawn(this);

    crosshair.oam.priority = 0;

    body = &body_idle;
    arms = &arms_idle;
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

    if(inventory[selected_slot] != nullptr)
    {
        Item* _item = inventory[selected_slot];
        _item->aim = aim;
        _item->update();

        if((keysHeld() & KEY_TOUCH))
            _item->use();
    }

    if(abs(velocity.length()) < 0.1)
    {
        body = &body_idle;
        arms = &arms_idle;

        frame = 0;
    }
    else
    {
        body = &body_walk;
        arms = &arms_walk;

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

    body->oam.position = position;
    arms->oam.position = position;

    body->oam.flip_h = flip;
    arms->oam.flip_h = flip;

    body->frame = frame;
    arms->frame = frame;

    if(inventory[selected_slot] != nullptr)
    {
        Item* _item = inventory[selected_slot];

        _item->sprite.oam.position = position;
        _item->sprite.oam.position.y += center.y;

        _item->sprite.draw_affine(_camera, weapon_rotation, {(flip ? 1.0 : -1.0), -1.0});
    }

    body->draw(_camera);
    arms->draw(_camera);

    crosshair.oam.position = crosshair_raw_position;
    crosshair.draw({0, 0});
}