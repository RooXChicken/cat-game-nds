#include <../entity/entity.h>

Entity::Entity()
{
    type = EntityType::EMPTY;
    spawn();
}

void Entity::spawn() {}
void Entity::update() {}
void Entity::draw(Vector2 _camera) {}