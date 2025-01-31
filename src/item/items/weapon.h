#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include <../../item/item.h>
#include <../../entity/entities/player.h>
#include <../../entity/entities/bullet.h>

class Weapon : public Item
{
    protected:
    public:
        int use_delay = 20;
        int use_cooldown = 0;

        // Weapon(ItemTypes::Type _type);
        virtual void spawn(Player* _player) override;
        bool can_swap() override;
        void update() override;
        void use() override;
        virtual void shoot();
};

#endif