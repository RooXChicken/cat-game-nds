#define __NDS__
#define ARM9

#include <nds.h>
#include <../src/assets/sprite.h>
#include <../build/sprite_kita.h>
#include <../build/sprite_bella_walk.h>
#include <../build/sprite_bella_idle.h>
#include <../build/sprite_bella_walk_arms.h>
#include <../build/sprite_bella_idle_arms.h>
#include <../build/sprite_crosshair.h>
#include <../build/sprite_treat_pistol.h>
#include <../build/sprite_catnip_launcher.h>

Sprite::Sprite() {}

Sprite::Sprite(SpriteType _type, int _id, int _palette)
{
    type = _type;
    // i use an id system to allow multiple sprites to use the same image
    if(_id == -1)
        id = LAST_ID++;
    else
        id = _id;

    palette = _palette;

    int _sprite_tiles_len, _sprite_pal_len;
    const unsigned short* _sprite_pal;

    // depending on the sprite type, load in the relevant data
    // (hard-coded for now. maybe will be dynamic if i can figure it out)
    switch(type)
    {
        case(KITA):
            size = SpriteSize_64x64;
            color_format = SpriteColorFormat_256Color;
            
            data = (u8*)sprite_kitaTiles;
            _sprite_tiles_len = sprite_kitaTilesLen;

            _sprite_pal = sprite_kitaPal;
            _sprite_pal_len = sprite_kitaPalLen;
            break;

        case(BELLA_WALK):
            size = SpriteSize_32x32;
            color_format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_walkTiles;
            _sprite_tiles_len = sprite_bella_walkTilesLen;

            _sprite_pal = sprite_bella_walkPal;
            _sprite_pal_len = sprite_bella_walkPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(BELLA_WALK_ARMS):
            size = SpriteSize_32x32;
            color_format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_walk_armsTiles;
            _sprite_tiles_len = sprite_bella_walk_armsTilesLen;

            _sprite_pal = sprite_bella_walk_armsPal;
            _sprite_pal_len = sprite_bella_walk_armsPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(BELLA_IDLE):
            size = SpriteSize_32x32;
            color_format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_idleTiles;
            _sprite_tiles_len = sprite_bella_idleTilesLen;

            _sprite_pal = sprite_bella_idlePal;
            _sprite_pal_len = sprite_bella_idlePalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(BELLA_IDLE_ARMS):
            size = SpriteSize_32x32;
            color_format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_idle_armsTiles;
            _sprite_tiles_len = sprite_bella_idle_armsTilesLen;

            _sprite_pal = sprite_bella_idle_armsPal;
            _sprite_pal_len = sprite_bella_idle_armsPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(CROSSHAIR):
            size = SpriteSize_8x8;
            color_format = SpriteColorFormat_16Color;

            data = (u8*)sprite_crosshairTiles;
            _sprite_tiles_len = sprite_crosshairTilesLen;

            _sprite_pal = sprite_crosshairPal;
            _sprite_pal_len = sprite_crosshairPalLen;
            break;

        case(TREAT_PISTOL):
            size = SpriteSize_32x8;
            color_format = SpriteColorFormat_16Color;

            data = (u8*)sprite_treat_pistolTiles;
            _sprite_tiles_len = sprite_treat_pistolTilesLen;

            _sprite_pal = sprite_treat_pistolPal;
            _sprite_pal_len = sprite_treat_pistolPalLen;
            break;

        case(CATNIP_CANON):
            size = SpriteSize_32x8;
            color_format = SpriteColorFormat_16Color;

            data = (u8*)sprite_catnip_launcherTiles;
            _sprite_tiles_len = sprite_catnip_launcherTilesLen;

            _sprite_pal = sprite_catnip_launcherPal;
            _sprite_pal_len = sprite_catnip_launcherPalLen;
            break;
    }

    // allocate memory and copy the tiles into the tile buffer
    allocate_memory();
    dmaCopy(data, pointer, _sprite_tiles_len);

    // if the palette is -1, it will allocate the sprite's palette into memory
    // if it is a number, then it indexes into the palette list instead of loading one in
    // (re-use palettes)
    if(palette == -1)
    {
        palette = LAST_PALLETE;
        dmaCopy(_sprite_pal, &SPRITE_PALETTE[palette*16], _sprite_pal_len);
        LAST_PALLETE++;
    }
}

void Sprite::allocate_memory()
{
    if(LOADED_TEX[type] == 0)
        LOADED_TEX[type] = oamAllocateGfx(&oamMain, size, color_format);

    pointer = LOADED_TEX[type];
}

void Sprite::make_affine(int _affine_id)
{
    if(_affine_id == -1)
    {
        for(int i = 0; i < MATRIX_COUNT; i++)
            if(AFFINE_ID[i] == 0)
            {
                AFFINE_ID[i] = pointer;
                affine_index = i;
            }
    }
    else
        affine_index = _affine_id;

    oamSetAffineIndex(&oamMain, id, affine_index, false);
}

void Sprite::_animate()
{
    if((int)frame != previous_frame)
    {
        // animation by copying the sprite's frame into the 'drawn' tile array
        int _frame = (int)frame % frame_count;
        if(_frame < 0)
            _frame = frame_count - abs(_frame);
            
        u8* offset = data + _frame * frame_size;
        dmaCopy(offset, pointer, frame_size);
    }
}

void Sprite::draw(Vector2 _camera)
{
    _animate();

    //copy the information to the OAM
    oamSet(&oamMain, id, (int)(position.x - _camera.x), (int)(position.y - _camera.y), priority, palette, size, color_format, pointer, affine_index, false, hide, flip_h, flip_v, mosaic);
    // oamSetPalette(&oamMain, id, palette);
}

void Sprite::draw_affine(Vector2 _camera, double _rotation, Vector2 _scale)
{
    //if the scale is 0, don't draw (mainly to avoid divide by zero)
    if(_scale.x == 0 || _scale.y == 0)
        return;

    oamRotateScale(&oamMain, 0, degreesToAngle((int)(_rotation*32767) % 32767), floatToFixed((float)(1/_scale.x), 8), floatToFixed((float)(1/_scale.y), 8));
    draw(_camera);
}

void Sprite::destroy(bool _global)
{
    if(_global)
    {
        oamFreeGfx(&oamMain, pointer);
        LOADED_TEX[type] = 0;
    }

    if(affine_index != -1)
    {
        AFFINE_ID[affine_index] = 0;
        affine_index = -1;
    }
}