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
#include <../build/sprite_cat_treat.h>

void OAMObject::make_affine(int _affine_id)
{
    if(_affine_id == -1)
    {
        for(int i = 0; i < MATRIX_COUNT; i++)
            if(AFFINE_ID[i] == 0)
            {
                AFFINE_ID[i] = this;
                affine_id = i;
            }
    }
    else
        affine_id = _affine_id;

    oamSetAffineIndex(&oamMain, id, affine_id, false);
}

void OAMObject::draw(Vector2 _camera)
{
    oamSet(&oamMain, id, 
    (int)(position.x - _camera.x), (int)(position.y - _camera.y), 
    priority, palette->id, size, palette->format, 
    pointer, affine_id, false, hide, flip_h, flip_v, mosaic);
}

void OAMObject::draw_affine(Vector2 _camera, double _rotation, Vector2 _scale)
{
    //if the scale is 0, don't draw (mainly to avoid divide by zero)
    if(_scale.x == 0 || _scale.y == 0)
        return;

    oamRotateScale(&oamMain, affine_id, degreesToAngle(-_rotation), floatToFixed((float)(1/_scale.x), 8), floatToFixed((float)(1/_scale.y), 8));
    draw(_camera);
}

void OAMObject::destroy()
{
    oamClearSprite(&oamMain, id);
    OAM_SLOTS[id] = nullptr;
    id = -1;

    if(affine_id != -1)
    {
        AFFINE_ID[affine_id] = nullptr;
        affine_id = -1;
    }

}

Sprite::Sprite() {}

Sprite::Sprite(SpriteType _type, int _id, int _palette)
{
    type = _type;

    int _sprite_tiles_len, _sprite_pal_len;
    const unsigned short* _sprite_pal;

    SpriteColorFormat _format;

    oam = new OAMObject{};
    oam->palette = new OAMPalette{};

    for(int i = 0; i < SPRITE_COUNT; i++)
    {
        if(OAM_SLOTS[i] == nullptr)
        {
            oam->id = i;
            break;
        }
    }

    // depending on the sprite type, load in the relevant data
    // (hard-coded for now. maybe will be dynamic if i can figure it out)
    switch(type)
    {
        case(KITA):
            oam->size = SpriteSize_64x64;
            _format = SpriteColorFormat_256Color;
            
            data = (u8*)sprite_kitaTiles;
            _sprite_tiles_len = sprite_kitaTilesLen;

            _sprite_pal = sprite_kitaPal;
            _sprite_pal_len = sprite_kitaPalLen;
            break;

        case(BELLA_WALK):
            oam->size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_walkTiles;
            _sprite_tiles_len = sprite_bella_walkTilesLen;

            _sprite_pal = sprite_bella_walkPal;
            _sprite_pal_len = sprite_bella_walkPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(BELLA_WALK_ARMS):
            oam->size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_walk_armsTiles;
            _sprite_tiles_len = sprite_bella_walk_armsTilesLen;

            _sprite_pal = sprite_bella_walk_armsPal;
            _sprite_pal_len = sprite_bella_walk_armsPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(BELLA_IDLE):
            oam->size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_idleTiles;
            _sprite_tiles_len = sprite_bella_idleTilesLen;

            _sprite_pal = sprite_bella_idlePal;
            _sprite_pal_len = sprite_bella_idlePalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(BELLA_IDLE_ARMS):
            oam->size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_idle_armsTiles;
            _sprite_tiles_len = sprite_bella_idle_armsTilesLen;

            _sprite_pal = sprite_bella_idle_armsPal;
            _sprite_pal_len = sprite_bella_idle_armsPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(CROSSHAIR):
            oam->size = SpriteSize_8x8;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_crosshairTiles;
            _sprite_tiles_len = sprite_crosshairTilesLen;

            _sprite_pal = sprite_crosshairPal;
            _sprite_pal_len = sprite_crosshairPalLen;
            break;

        case(TREAT_PISTOL):
            oam->size = SpriteSize_32x8;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_treat_pistolTiles;
            _sprite_tiles_len = sprite_treat_pistolTilesLen;

            _sprite_pal = sprite_treat_pistolPal;
            _sprite_pal_len = sprite_treat_pistolPalLen;
            break;

        case(CATNIP_CANON):
            oam->size = SpriteSize_32x8;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_catnip_launcherTiles;
            _sprite_tiles_len = sprite_catnip_launcherTilesLen;

            _sprite_pal = sprite_catnip_launcherPal;
            _sprite_pal_len = sprite_catnip_launcherPalLen;
            break;

        case(CAT_TREAT):
            oam->size = SpriteSize_8x8;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_cat_treatTiles;
            _sprite_tiles_len = sprite_cat_treatTilesLen;

            _sprite_pal = sprite_cat_treatPal;
            _sprite_pal_len = sprite_cat_treatPalLen;
            break;
    }

    // allocate memory and copy the tiles into the tile buffer
    allocate_memory();
    dmaCopy(data, oam->pointer, _sprite_tiles_len);

    // if the palette is -1, it will allocate the sprite's palette into memory
    // if it is a number, then it indexes into the palette list instead of loading one in
    // (re-use palettes)
    if(_palette == -1)
    {
        for(int i = 0; i < 16; i++)
            if(PALETTE_SLOTS[i] == nullptr)
            {
                oam->palette = new OAMPalette{i, SpriteColorFormat_16Color};
                break;
            }

        dmaCopy(_sprite_pal, &SPRITE_PALETTE[oam->palette->id*16], _sprite_pal_len);
    }
    else
        oam->palette = PALETTE_SLOTS[_palette];

    OAM_SLOTS[oam->id] = oam;
    PALETTE_SLOTS[oam->palette->id] = oam->palette;
}

void Sprite::allocate_memory()
{
    if(LOADED_TEX[type] == nullptr)
        LOADED_TEX[type] = oamAllocateGfx(&oamMain, oam->size, oam->palette->format);

    oam->pointer = LOADED_TEX[type];
}

void Sprite::_animate()
{
    if((int)frame != previous_frame)
    {
        // animation by copying the sprite's frame into the 'drawn' tile array
        int _frame = (int)frame % frame_count;
        if(_frame < 0)
            _frame = frame_count - abs(_frame);
            
        u8* offset = data + (_frame * frame_size);
        dmaCopy(offset, oam->pointer, frame_size);

        previous_frame = (int)frame;
    }
}

void Sprite::draw(Vector2 _camera)
{
    _animate();
    oam->draw(_camera);
}

void Sprite::draw_affine(Vector2 _camera, double _rotation, Vector2 _scale)
{
    _animate();
    oam->draw_affine(_camera, _rotation, _scale);
}

void Sprite::destroy(bool _global)
{
    if(_global)
    {
        oamFreeGfx(&oamMain, oam->pointer);
        LOADED_TEX[type] = nullptr;
        oam->pointer = nullptr;
    }

    oam->destroy();
}