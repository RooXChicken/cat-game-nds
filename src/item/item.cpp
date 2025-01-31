#include <../item/item.h>

Item::Item()
{
    // type = ItemType::EMPTY;
}

void Item::spawn(Player* _player)
{
    player = _player;
}

void Item::update() {}
void Item::use() {}
void Item::draw(Vector2 _camera) {}

void Item::destroy()
{
    if(use_sound != nullptr)
        use_sound->destroy();

    sprite.destroy(false);
}