#include <../items/weapon.h>

void Weapon::spawn(Player* _player)
{
    player = _player;
    sprite = Sprite(SpriteType::TREAT_PISTOL, 0, -1);
    sprite.oam.make_affine(-1);
}

void Weapon::update()
{
    if(use_cooldown > 0)
        use_cooldown--;
}

void Weapon::use()
{
    if(use_cooldown <= 0)
    {
        use_cooldown = use_delay;
        shoot();
    }
}

void Weapon::shoot()
{
    Bullet* _bullet = (Bullet*)player->scene->spawn_entity(new Bullet());
    _bullet->position = player->position + player->center;
    _bullet->velocity = aim * -3;
}