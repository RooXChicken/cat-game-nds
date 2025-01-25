#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#define __NDS__
#define ARM9

#include <nds.h>
#include <stdio.h>

#include <../math/vector2.h>

enum SpriteType
{
    KITA = 0,
    BELLA_WALK = 1,
    BELLA_WALK_ARMS = 2,
    BELLA_IDLE = 3,
    BELLA_IDLE_ARMS = 4,
    CROSSHAIR = 5,
    TREAT_PISTOL = 6,
    CATNIP_CANON = 7
};

static int LAST_ID = 0;
static int LAST_PALLETE = 0;

static u16* AFFINE_ID[32];
static u16* LOADED_TEX[256];

class Sprite
{
    private:
        SpriteType type;

        void allocate_memory();
        void _animate();
        int previous_frame = 0;
    public:
        u16* pointer;
        u8* data;
        int id;
        int affine_index = -1;

        SpriteSize size;
        SpriteColorFormat color_format;
        int alpha_color = 0xFF00FF;
        int palette = -1;

        int priority = 1;
        Vector2 position;

        int frame_count = 1;
        int frame_size = 32*16; //needs to be half of what it *should be*. don't yet know why
        double frame = 0;

        bool hide = false;
        bool flip_h = false;
        bool flip_v = false;
        bool mosaic = false;

        Sprite();
        Sprite(SpriteType _type, int _id, int _palette);

        void make_affine(int _affine_id);

        void draw(Vector2 _camera);
        void draw_affine(Vector2 _camera, double _rotation, Vector2 _scale);
        void destroy(bool _global);
};

#endif