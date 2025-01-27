#ifndef TREAT_PISTOL_WEAPON_H_INCLUDED
#define TREAT_PISTOL_WEAPON_H_INCLUDED

#include <../../item/item.h>
#include <../../item/items/weapon.h>
#include <../../entity/entities/player.h>
#include <../../entity/entities/bullet.h>

class TreatPistolWeapon : public Weapon
{
    public:
        void spawn(Player* _player) override;
        void shoot() override;
};

#endif