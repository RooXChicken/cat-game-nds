#ifndef MAIN_GAME_SCENE_H_INCLUDED
#define MAIN_GAME_SCENE_H_INCLUDED

#include <../../scene/scene.h>
#include <../../entity/entity.h>
#include <../../entity/entities/player.h>

class MainGameScene : public Scene
{
    private:
    public:
        // MainGameScene();
        void load() override;

        void update() override;
    
        // void unload() override;
};

void MainGameScene::load()
{
    spawn_entity(new Player());
}

void MainGameScene::update()
{
    for(int i = 0; i < 128; i++)
    {
        if(entities[i] != nullptr)
            entities[i]->update();
    }

    camera_position.x = entities[0]->position.x - (SCREEN_WIDTH/2);
    camera_position.y = entities[0]->position.y - (SCREEN_HEIGHT/2);
}

#endif