#define __NDS__
#define ARM9

#include <nds.h>
#include <../source/sprite.h>
#include <../gfx/sprite_kita.h>
#include <../gfx/sprite_square.h>
#include <../gfx/sprite_bella.h>
#include <../gfx/sprite_bella_walk_arms.h>

Sprite::Sprite() {}

Sprite::Sprite(SpriteType _type, int _id, int _palette)
{
    if(_id == -1)
        id = LAST_ID++;
    else
        id = _id;

    palette = _palette;

    int _sprite_tiles_len, _sprite_pal_len;
    const unsigned short* _sprite_pal;

    switch(_type)
    {
        case(KITA):
            size = SpriteSize_64x64;
            color_format = SpriteColorFormat_256Color;
            
            allocate_memory(_type);

            data = (u8*)sprite_kitaTiles;
            _sprite_tiles_len = sprite_kitaTilesLen;

            _sprite_pal = sprite_kitaPal;
            _sprite_pal_len = sprite_kitaPalLen;
            break;

        case(SQUARE):
            size = SpriteSize_16x16;
            color_format = SpriteColorFormat_16Color;

            allocate_memory(_type);
            
            data = (u8*)sprite_squareTiles;
            _sprite_tiles_len = sprite_squareTilesLen;

            _sprite_pal = sprite_squarePal;
            _sprite_pal_len = sprite_squarePalLen;
            break;

        case(BELLA):
            size = SpriteSize_32x32;
            color_format = SpriteColorFormat_16Color;

            allocate_memory(_type);

            data = (u8*)sprite_bellaTiles;
            _sprite_tiles_len = sprite_bellaTilesLen;

            _sprite_pal = sprite_bellaPal;
            _sprite_pal_len = sprite_bellaPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(BELLA_ARMS):
            size = SpriteSize_32x32;
            color_format = SpriteColorFormat_16Color;
            
            allocate_memory(_type);

            data = (u8*)sprite_bella_walk_armsTiles;
            _sprite_tiles_len = sprite_bella_walk_armsTilesLen;

            _sprite_pal = sprite_bella_walk_armsPal;
            _sprite_pal_len = sprite_bella_walk_armsPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;
    }

    dmaCopy(data, pointer, _sprite_tiles_len);

    if(palette == -1)
    {
        palette = id;
        dmaCopy(_sprite_pal, &SPRITE_PALETTE[palette*16], _sprite_pal_len);
    }
}

void Sprite::allocate_memory(SpriteType _type)
{
    if(LOADED_TEX[_type] == 0)
        LOADED_TEX[_type] = oamAllocateGfx(&oamMain, size, color_format);

    pointer = LOADED_TEX[_type];
}

void Sprite::draw(double _camera_x, double _camera_y)
{
    if((int)frame != previous_frame)
    {
        u8* offset = data + ((int)frame % frame_count) * frame_size;
        dmaCopy(offset, pointer, frame_size);
    }

    oamSet(&oamMain, id, (int)(x - _camera_x), (int)(y - _camera_y), priority, 0, size, color_format, pointer, -1, false, hide, flip_h, flip_v, mosaic);
}