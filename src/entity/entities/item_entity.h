#ifndef ITEM_ENTITY_H_INCLUDED
#define ITEM_ENTITY_H_INCLUDED

#include <../../entity/entity.h>
#include <../../item/item.h>

class ItemEntity : public Entity
{
    private:
    public:
        Item* item = nullptr;
        Vector2 velocity = {0, 0};
        double decel_speed = 0.04;
        double damage = 0;

        ItemEntity(Item* _item, ItemTypes::Type _type);
        void spawn(Scene* _scene, int _id) override;
        void update(Vector2 _camera) override;
        void draw(Vector2 _camera) override;
};

#endif