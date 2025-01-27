#include <../scene/main_game.h>

void MainGameScene::load()
{
    spawn_entity(new Player());
}

void MainGameScene::update()
{
    for(int i = 0; i < 128; i++)
    {
        if(entities[i] == nullptr)
            continue;

        entities[i]->update(camera_position);
    }

    camera_position.x = entities[0]->position.x - (SCREEN_WIDTH/2) + 12;
    camera_position.y = entities[0]->position.y - (SCREEN_HEIGHT/2) + 16;
}