#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#define __NDS__
#define ARM9

#include <nds.h>
#include <stdio.h>

enum SpriteType
{
    KITA = 0,
    SQUARE = 1,
    BELLA = 2,
    BELLA_ARMS = 3
};

static int LAST_ID = 0;
static u16* LOADED_TEX[256];

class Sprite
{
    private:
        void allocate_memory(SpriteType _type);
        int previous_frame = 0;
    public:
        u16* pointer;
        u8* data;
        int id;

        SpriteSize size;
        SpriteColorFormat color_format;
        int alpha_color = 0xFF00FF;
        int palette = -1;

        int priority = 0;
        double x = 0;
        double y = 0;

        int frame_count = 1;
        int frame_size = 32*16; //needs to be half of what it *should be*. don't yet know why
        double frame = 0;

        bool hide = false;
        bool flip_h = false;
        bool flip_v = false;
        bool mosaic = false;

        Sprite();
        Sprite(SpriteType _type, int _id, int _palette);
        void draw(double _camera_x, double _camera_y);
};

#endif