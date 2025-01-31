#ifndef MAIN_GAME_SCENE_H_INCLUDED
#define MAIN_GAME_SCENE_H_INCLUDED

#include <../../scene/scene.h>
#include <../../entity/entity.h>
#include <../../entity/entities/player.h>
#include <../../entity/entities/item_entity.h>

class MainGameScene : public Scene
{
    private:
    public:
        void load() override;

        void update() override;
    
        // void unload() override;
};

#endif