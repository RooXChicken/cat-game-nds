#include <../items/weapon.h>

void Weapon::spawn(Player* _player)
{
    player = _player;
    sprite = Sprite(SpriteTypes::Type::TREAT_PISTOL, 0, -1);
    sprite.oam.make_affine(-1);
}

bool Weapon::can_swap()
{
    return use_cooldown <= 0;
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
    // Bullet* _bullet = (Bullet*)player->scene->spawn_entity(new Bullet(Sprite(SpriteType::CAT_TREAT, -1, 1), 1.0));
    // _bullet->position = player->position + player->center;
    // _bullet->velocity = aim * -3;
}