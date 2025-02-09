#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#define __NDS__
#define ARM9

#include <nds.h>
#include <stdio.h>

#include <../math/vector2.h>

struct SpriteTypes
{
    enum Type
    {
        KITA = 0,
        BELLA_WALK = 1,
        BELLA_WALK_ARMS = 2,
        BELLA_IDLE = 3,
        BELLA_IDLE_ARMS = 4,
        CROSSHAIR = 5,
        TREAT_PISTOL = 6,
        CATNIP_CANON = 7,
        CAT_TREAT = 8,
        BELLA_SHOOT_D = 9,
        BELLA_SHOOT_D2 = 10,
        BELLA_SHOOT_M = 11,
        BELLA_SHOOT_M2 = 12,
        BELLA_SHOOT_U = 13,
        BELLA_SHOOT_U2 = 14,
        QTIP_RIFLE = 15,
        QTIP = 16,
        BANANA_BREAD = 17,
        BOBA_TEA = 18,
        BREAD = 19,
        CHALK = 20,
        CINNAMON = 21,
        CLOTHING_TAG = 22,
        COFFEE = 23,
        LUCKY_BAG = 24,
        NEW_WORLD = 25,
        PINEAPPLE = 26,
        MOCHI = 27,
        PEANUT_BUTTER_APPLE = 28,
        POLAROID = 29,
        WORN_TOY_CAT = 30
        // SHADOW = 17
    };
};

struct OAMPalette
{
    int id = 0;
    SpriteColorFormat format = SpriteColorFormat_16Color;
    int alpha_color = 0xFF00FF;
};

struct OAMObject
{
    int oam_id = -1;
    int affine_id = -1;

    bool static_slot = false;

    u16* pointer = nullptr;
    OAMPalette* palette = nullptr;
    u8 priority = 1;

    Vector2 position = {0, 0};
    Vector2 camera_offset = {0, 0};
    SpriteSize size = SpriteSize_8x8;

    bool hide = false;
    Vector2 mosaic = {0, 0};
    bool flip_h = false;
    bool flip_v = false;

    void make_affine(int _affine_id);

    void draw(Vector2 _camera);
    void draw_affine(Vector2 _camera, double _rotation, Vector2 _size);

    void destroy();
};

struct SpriteData
{
    SpriteSize size;
    SpriteColorFormat format;

    u8* data;
    int tiles_length;

    const unsigned short* palette;
    u32 palette_length;

    int frame_count = 1;
    int frame_size = 0;
};

class Sprite
{
    private:
        SpriteTypes::Type type;

        void allocate_memory(SpriteColorFormat _format);
        void _animate();
        int previous_frame = 0;
    public:
        OAMObject oam;
        u8* data;

        int frame_count = 1;
        int frame_size = 32*16; //needs to be half of what it *should be*. don't yet know why
        double frame = 0;

        Sprite();
        Sprite(SpriteTypes::Type _type, int _oam_id, int _palette);

        void draw(Vector2 _camera);
        void draw_affine(Vector2 _camera, double _rotation, Vector2 _scale);
        void destroy(bool _global);

        static void _get_sprite_data(SpriteTypes::Type _type, SpriteData* _data);
        static void load_palette(SpriteTypes::Type _type, int _index);
        static void _display();
};

#endif