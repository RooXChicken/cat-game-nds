#include <../scene/scene.h>

Scene::Scene()
{
    load();
}

void Scene::load() {}

void Scene::spawn_entity(Entity* _entity)
{
	for(int i = 0; i < 128; i++)
	{
		if(entities[i] == nullptr)
		{
			entities[i] = _entity;
			entities[i]->spawn();
			return;
		}
	}
}

void Scene::update()
{
    for(int i = 0; i < 128; i++)
    {
        if(entities[i] != nullptr)
            entities[i]->update();
    }
}

void Scene::render()
{
    for(int i = 0; i < 128; i++)
    {
        if(entities[i] != nullptr)
            entities[i]->draw(camera_position);
    }
}

void Scene::unload()
{
    for(int i = 0; i < 128; i++)
    {
        entities[i]->destroy();
        entities[i] = nullptr;
    }
}