#include <../item/item.h>

Item::Item()
{
    type = ItemType::EMPTY;
    spawn();
}

void Item::spawn() {}
void Item::update() {}
void Item::draw(Vector2 _camera) {}