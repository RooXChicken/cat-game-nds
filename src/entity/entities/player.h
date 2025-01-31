#ifndef PLAYER_ENTITY_H_INCLUDED
#define PLAYER_ENTITY_H_INCLUDED

#include <../../entity/entity.h>
#include <../../item/item.h>

class Item;

class Player : public Entity
{
    private:
        Sprite body_idle;
        Sprite body_walk;

        Sprite arms_idle;
        Sprite arms_walk;

        Sprite shoot_d;
        Sprite shoot_d2;
        Sprite shoot_m;
        Sprite shoot_m2;
        Sprite shoot_u;
        Sprite shoot_u2;

        Sprite* body;
        Sprite* arms;
        Sprite* arms2;

        Sprite crosshair;
        Vector2 crosshair_raw_position;

        Item* inventory[16];
        int queued_slot = 0;
        int selected_slot = 0;

        bool flip;
        double frame = 0;

        double move_speed = 0.5;
        double decel_speed = 0.2;
        double speed_cap = 2;

        Vector2 move_dir = {0, 0};

        Vector2 aim = {0, 0};
        double weapon_rotation = 0;

        void _move();
        int shoot_cooldown = 0;

    public:
        Vector2 center;
        
        void spawn(Scene* _scene, int _id) override;
        void update(Vector2 _camera) override;
        void draw(Vector2 _camera) override;
};

#endif