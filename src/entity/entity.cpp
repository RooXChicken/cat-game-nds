#include <../entity/entity.h>

Entity::Entity()
{
    type = EntityType::EMPTY;
}

void Entity::spawn(Scene* _scene, int _id)
{
    scene = _scene;
    id = _id;
}

void Entity::update(Vector2 _camera) {}
void Entity::draw(Vector2 _camera) {}

void Entity::destroy()
{
    sprite.destroy(true);
    scene->entities[id] = nullptr;
}