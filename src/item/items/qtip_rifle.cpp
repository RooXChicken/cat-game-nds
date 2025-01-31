#include <../items/qtip_rifle.h>

void QTipRifleWeapon::spawn(Player* _player)
{
    player = _player;
    sprite = Sprite(SpriteType::QTIP_RIFLE, 0, -1);
    sprite.oam.make_affine(0);

    use_sound = new Sound(SFX_GUN_SHOOT);

    use_delay = 12;
}

void QTipRifleWeapon::shoot()
{
    Bullet* _bullet = (Bullet*)player->scene->spawn_entity(new Bullet(Sprite(SpriteType::QTIP, -1, 1), 1.0));
    _bullet->position = player->position + player->center;
    _bullet->velocity = aim * -8;
    _bullet->decel_speed = 0.1;

    use_sound->play(false);
}