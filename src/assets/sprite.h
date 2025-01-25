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
    CATNIP_CANON = 7,
    CAT_TREAT = 8
};

struct OAMPalette
{
    int id = 0;
    SpriteColorFormat format = SpriteColorFormat_16Color;
    int alpha_color = 0xFF00FF;
};

struct OAMObject
{
    int id = 0;
    int affine_id = -1;
    u16* pointer = nullptr;

    OAMPalette* palette = nullptr;
    u8 priority = 1;

    Vector2 position = {0, 0};
    SpriteSize size = SpriteSize_8x8;

    bool hide = false;
    bool mosaic = false;
    bool flip_h = false;
    bool flip_v = false;

    void make_affine(int _affine_id);

    void draw(Vector2 _camera);
    void draw_affine(Vector2 _camera, double _rotation, Vector2 _size);

    void destroy();
};

static u16* LOADED_TEX[256];

static OAMObject* OAM_SLOTS[SPRITE_COUNT];
static OAMObject* AFFINE_ID[MATRIX_COUNT];

static OAMPalette* PALETTE_SLOTS[16];

class Sprite
{
    private:
        SpriteType type;

        void allocate_memory();
        void _animate();
        int previous_frame = 0;
    public:
        OAMObject* oam;
        u8* data;

        int frame_count = 1;
        int frame_size = 32*16; //needs to be half of what it *should be*. don't yet know why
        double frame = 0;

        Sprite();
        Sprite(SpriteType _type, int _id, int _palette);

        void draw(Vector2 _camera);
        void draw_affine(Vector2 _camera, double _rotation, Vector2 _scale);
        void destroy(bool _global);
};

#endif