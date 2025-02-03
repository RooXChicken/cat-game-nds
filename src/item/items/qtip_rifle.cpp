#include <../items/qtip_rifle.h>

void QTipRifleWeapon::spawn(Player* _player)
{
    player = _player;
    _spawn(ItemTypes::Type::QTIP_RIFLE);

    sprite.oam.static_slot = true;
    sprite.oam.oam_id = 0;
    sprite.oam.make_affine(0);

    use_sound = new Sound(SFX_GUN_SHOOT);

    use_delay = 12;
}

void QTipRifleWeapon::_spawn(ItemTypes::Type _type)
{
    type = _type;
    sprite = Sprite(SpriteTypes::Type::QTIP_RIFLE, -1, 1);
}

void QTipRifleWeapon::shoot()
{
    Bullet* _bullet = (Bullet*)player->scene->spawn_entity(new Bullet(Sprite(SpriteTypes::Type::QTIP, -1, 1), 1.0));
    _bullet->position = player->position + player->center;
    _bullet->velocity = aim * -8;
    _bullet->decel_speed = 0.1;

    use_sound->play(false);
}