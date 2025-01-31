#include <../entities/item_entity.h>

ItemEntity::ItemEntity(Item* _item, ItemTypes::Type _type)
{
    item = _item;
    item->_spawn(_type);
}

void ItemEntity::spawn(Scene* _scene, int _id)
{
    type = EntityTypes::Type::ITEM_ENTITY;
    scene = _scene;
    id = _id;
}

void ItemEntity::update(Vector2 _camera)
{
    
}

void ItemEntity::draw(Vector2 _camera)
{
    item->sprite.oam.position = position;
    item->sprite.draw(_camera);
}