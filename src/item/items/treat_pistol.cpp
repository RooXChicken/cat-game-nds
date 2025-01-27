#include <../items/treat_pistol.h>

void TreatPistolWeapon::spawn(Player* _player)
{
    player = _player;
    sprite = Sprite(SpriteType::TREAT_PISTOL, 0, -1);
    sprite.oam.make_affine(0);
}

void TreatPistolWeapon::shoot()
{
    Bullet* _bullet = (Bullet*)player->scene->spawn_entity(new Bullet(Sprite(SpriteType::CAT_TREAT, -1, 1), 1.0));
    _bullet->position = player->position + player->center;
    _bullet->velocity = aim * -3;
}