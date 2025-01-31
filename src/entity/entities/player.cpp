#include <../entities/player.h>
#include <../entities/bullet.h>
#include <../../item/items/weapon.h>
#include <../../item/items/treat_pistol.h>
#include <../../item/items/qtip_rifle.h>

void Player::spawn(Scene* _scene, int _id)
{
    type = EntityTypes::Type::PLAYER;
    scene = _scene;
    id = _id;

    arms_idle = Sprite(SpriteTypes::Type::BELLA_IDLE_ARMS, 126, -1);
    arms_walk = Sprite(SpriteTypes::Type::BELLA_WALK_ARMS, 126, 0);

    body_idle = Sprite(SpriteTypes::Type::BELLA_IDLE, 127, 0);
    body_walk = Sprite(SpriteTypes::Type::BELLA_WALK, 127, 0);

    shoot_d = Sprite(SpriteTypes::Type::BELLA_SHOOT_D, 126, 0);
    shoot_d2 = Sprite(SpriteTypes::Type::BELLA_SHOOT_D2, 125, 0);
    shoot_m = Sprite(SpriteTypes::Type::BELLA_SHOOT_M, 126, 0);
    shoot_m2 = Sprite(SpriteTypes::Type::BELLA_SHOOT_M2, 125, 0);
    shoot_u = Sprite(SpriteTypes::Type::BELLA_SHOOT_U, 126, 0);
    shoot_u2 = Sprite(SpriteTypes::Type::BELLA_SHOOT_U2, 125, 0);

    Sprite::load_palette(SpriteTypes::Type::TREAT_PISTOL, 1);

    shadow = Sprite(SpriteTypes::Type::SHADOW, -1, -2);

    shoot_d.oam.priority = 0;
    shoot_m.oam.priority = 0;
    shoot_u.oam.priority = 0;

    crosshair = Sprite(SpriteTypes::Type::CROSSHAIR, 1, 0);
    center = {12, 16};

    inventory[0] = (Item*)(new QTipRifleWeapon());
    inventory[0]->spawn(this);

    inventory[1] = (Item*)(new TreatPistolWeapon());
    inventory[1]->spawn(this);

    crosshair.oam.priority = 0;

    body = &body_idle;
    arms = &arms_idle;
    arms2 = &shoot_m2;
    arms2->oam.hide = true;
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

    if(keysDown() & KEY_L)
        queued_slot--;

    if(keysDown() & KEY_R)
        queued_slot++;

    if(queued_slot < 0)
        queued_slot += 16;
    if(queued_slot > 15)
        queued_slot -= 16;

    if(inventory[selected_slot] == nullptr)
    {
        selected_slot = queued_slot;
    }
    else if(inventory[selected_slot]->can_swap())
    {
        inventory[selected_slot]->sprite.oam.hide = true;
        selected_slot = queued_slot;
    }

    if(inventory[selected_slot] != nullptr)
    {
        Item* _item = inventory[selected_slot];
        _item->sprite.oam.hide = false;

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

    if(inventory[selected_slot] != nullptr)
    {
        double _rotation = abs(weapon_rotation);

        if(_rotation < 145)
        {
            arms = &shoot_u;
            arms2 = &shoot_u2;
        }
        else if(_rotation > 215)
        {
            arms = &shoot_d;
            arms2 = &shoot_d2;
        }
        else if(_rotation >= 145 && weapon_rotation <= 215)
        {
            arms = &shoot_m;
            arms2 = &shoot_m2;
        }
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
    arms2->oam.position = position;

    body->oam.flip_h = flip;
    arms->oam.flip_h = flip;
    arms2->oam.flip_h = flip;

    body->frame = frame;
    if(inventory[selected_slot] == nullptr)
        arms->frame = frame;

    if(inventory[selected_slot] != nullptr)
    {
        Item* _item = inventory[selected_slot];

        _item->sprite.oam.position = position;
        _item->sprite.oam.position.y += 2;

        _item->sprite.draw_affine(_camera, weapon_rotation, {(flip ? 1.0 : -1.0), -1.0});
    }

    // shadow.oam.position = position;
    // shadow.draw(_camera);

    body->draw(_camera);
    arms->draw(_camera);

    arms2->oam.hide = (inventory[selected_slot] == nullptr);
    arms2->draw(_camera);

    crosshair.oam.position = crosshair_raw_position;
    crosshair.draw({0, 0});
}