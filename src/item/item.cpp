#include <../item/item.h>

Item::Item(ItemTypes::Type _type)
{
    type = _type;
    switch(type)
    {
        case ItemTypes::Type::TREAT_PISTOL:
            sprite = Sprite(SpriteTypes::Type::TREAT_PISTOL, -1, 1);
            break;
            
        case ItemTypes::Type::QTIP_RIFLE:
            sprite = Sprite(SpriteTypes::Type::QTIP_RIFLE, -1, 1);
            break;
    }
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