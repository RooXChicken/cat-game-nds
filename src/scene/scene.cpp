#include <../scene/scene.h>

Scene::Scene() {}
void Scene::load() {}

Entity* Scene::spawn_entity(Entity* _entity)
{
	for(int i = 0; i < 128; i++)
	{
		if(entities[i] == nullptr)
		{
			entities[i] = _entity;
			entities[i]->spawn(this, i);
			return entities[i];
		}
	}

    return nullptr;
}

void Scene::update()
{
    for(int i = 0; i < 128; i++)
    {
        if(entities[i] == nullptr)
            continue;

        entities[i]->update(camera_position);
    }
}

void Scene::render()
{
    for(int i = 0; i < 128; i++)
    {
        if(entities[i] == nullptr)
            continue;

        entities[i]->draw(camera_position);
    }
}

void Scene::unload()
{
    for(int i = 0; i < 128; i++)
    {
        if(entities[i] == nullptr)
            continue;

        entities[i]->destroy();
        entities[i] = nullptr;
    }

    free(this);
}