#include <../src/assets/sprite.h>
#include <../build/sprite_kita.h>
#include <../build/sprite_bella_walk.h>
#include <../build/sprite_bella_idle.h>
#include <../build/sprite_bella_walk_arms.h>
#include <../build/sprite_bella_idle_arms.h>
#include <../build/sprite_bella_shoot_d.h>
#include <../build/sprite_bella_shoot_d2.h>
#include <../build/sprite_bella_shoot_m.h>
#include <../build/sprite_bella_shoot_m2.h>
#include <../build/sprite_bella_shoot_u.h>
#include <../build/sprite_bella_shoot_u2.h>
#include <../build/sprite_crosshair.h>
#include <../build/sprite_treat_pistol.h>
#include <../build/sprite_catnip_launcher.h>
#include <../build/sprite_cat_treat.h>
#include <../build/sprite_qtip.h>
#include <../build/sprite_qtip_rifle.h>
#include <../build/sprite_shadow.h>

static u16* LOADED_TEX[256];

static OAMObject* OAM_SLOTS[SPRITE_COUNT];
static OAMObject* AFFINE_ID[MATRIX_COUNT];

static OAMPalette* PALETTE_SLOTS[16];

void OAMObject::make_affine(int _affine_id)
{
    if(_affine_id == -1)
    {
        for(int i = 0; i < MATRIX_COUNT; i++)
            if(AFFINE_ID[i] == nullptr)
            {
                AFFINE_ID[i] = this;
                affine_id = i;
            }
    }
    else
        affine_id = _affine_id;

    oamSetAffineIndex(&oamMain, oam_id, affine_id, false);
}

// int oam_draw_index = 0;
void OAMObject::draw(Vector2 _camera)
{
    Vector2 _screen_pos = position - _camera;
    if(_screen_pos.x < -64 || _screen_pos.y < -64 || _screen_pos.x > SCREEN_WIDTH || _screen_pos.y > SCREEN_HEIGHT)
        return;

    camera_offset = _camera;

    if(!static_slot)
    {
        oam_id = SPRITE_COUNT;
        for(int i = 0; i < SPRITE_COUNT; i++)
            if(OAM_SLOTS[i] == nullptr)
            {
                oam_id = i;
                break;
            }

        if(oam_id >= SPRITE_COUNT)
            return;
    }

    OAM_SLOTS[oam_id] = this;
}

void OAMObject::draw_affine(Vector2 _camera, double _rotation, Vector2 _scale)
{
    //if the scale is 0, don't draw (mainly to avoid divide by zero)
    if(_scale.x == 0 || _scale.y == 0 || affine_id == -1)
        return;

    oamRotateScale(&oamMain, affine_id, degreesToAngle(-_rotation), floatToFixed((float)(1/_scale.x), 8), floatToFixed((float)(1/_scale.y), 8));
    draw(_camera);
}

void OAMObject::destroy()
{
    if(affine_id != -1)
    {
        AFFINE_ID[affine_id] = nullptr;
        affine_id = -1;
    }

    free(this);
}

Sprite::Sprite() {}

Sprite::Sprite(SpriteTypes::Type _type, int _oam_id, int _palette)
{
    type = _type;

    int _sprite_tiles_len, _sprite_pal_len;
    const unsigned short* _sprite_pal;

    SpriteColorFormat _format;

    oam = OAMObject{};
    // oam.palette = new OAMPalette{};

    if(_oam_id != -1)
    {
        oam.static_slot = true;
        oam.oam_id = _oam_id;
    }

    // depending on the sprite type, load in the relevant data
    // (hard-coded for now. maybe will be dynamic if i can figure it out)
    switch(type)
    {
        case(SpriteTypes::Type::KITA):
            oam.size = SpriteSize_64x64;
            _format = SpriteColorFormat_256Color;
            
            data = (u8*)sprite_kitaTiles;
            _sprite_tiles_len = sprite_kitaTilesLen;

            _sprite_pal = sprite_kitaPal;
            _sprite_pal_len = sprite_kitaPalLen;
            break;

        case(SpriteTypes::Type::BELLA_WALK):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_walkTiles;
            _sprite_tiles_len = sprite_bella_walkTilesLen;

            _sprite_pal = sprite_bella_walkPal;
            _sprite_pal_len = sprite_bella_walkPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_WALK_ARMS):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_walk_armsTiles;
            _sprite_tiles_len = sprite_bella_walk_armsTilesLen;

            _sprite_pal = sprite_bella_walk_armsPal;
            _sprite_pal_len = sprite_bella_walk_armsPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_IDLE):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_idleTiles;
            _sprite_tiles_len = sprite_bella_idleTilesLen;

            _sprite_pal = sprite_bella_idlePal;
            _sprite_pal_len = sprite_bella_idlePalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_IDLE_ARMS):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_idle_armsTiles;
            _sprite_tiles_len = sprite_bella_idle_armsTilesLen;

            _sprite_pal = sprite_bella_idle_armsPal;
            _sprite_pal_len = sprite_bella_idle_armsPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(SpriteTypes::Type::CROSSHAIR):
            oam.size = SpriteSize_8x8;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_crosshairTiles;
            _sprite_tiles_len = sprite_crosshairTilesLen;

            _sprite_pal = sprite_crosshairPal;
            _sprite_pal_len = sprite_crosshairPalLen;
            break;

        case(SpriteTypes::Type::TREAT_PISTOL):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_treat_pistolTiles;
            _sprite_tiles_len = sprite_treat_pistolTilesLen;

            _sprite_pal = sprite_treat_pistolPal;
            _sprite_pal_len = sprite_treat_pistolPalLen;
            break;

        case(SpriteTypes::Type::CATNIP_CANON):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_catnip_launcherTiles;
            _sprite_tiles_len = sprite_catnip_launcherTilesLen;

            _sprite_pal = sprite_catnip_launcherPal;
            _sprite_pal_len = sprite_catnip_launcherPalLen;
            break;

        case(SpriteTypes::Type::CAT_TREAT):
            oam.size = SpriteSize_8x8;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_cat_treatTiles;
            _sprite_tiles_len = sprite_cat_treatTilesLen;

            _sprite_pal = sprite_cat_treatPal;
            _sprite_pal_len = sprite_cat_treatPalLen;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_D):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_shoot_dTiles;
            _sprite_tiles_len = sprite_bella_shoot_dTilesLen;

            _sprite_pal = sprite_bella_shoot_dPal;
            _sprite_pal_len = sprite_bella_shoot_dPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_D2):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_shoot_d2Tiles;
            _sprite_tiles_len = sprite_bella_shoot_d2TilesLen;

            _sprite_pal = sprite_bella_shoot_d2Pal;
            _sprite_pal_len = sprite_bella_shoot_d2PalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_M):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_shoot_mTiles;
            _sprite_tiles_len = sprite_bella_shoot_mTilesLen;

            _sprite_pal = sprite_bella_shoot_mPal;
            _sprite_pal_len = sprite_bella_shoot_mPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_M2):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_shoot_m2Tiles;
            _sprite_tiles_len = sprite_bella_shoot_m2TilesLen;

            _sprite_pal = sprite_bella_shoot_m2Pal;
            _sprite_pal_len = sprite_bella_shoot_m2PalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_U):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_shoot_uTiles;
            _sprite_tiles_len = sprite_bella_shoot_uTilesLen;

            _sprite_pal = sprite_bella_shoot_uPal;
            _sprite_pal_len = sprite_bella_shoot_uPalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_U2):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_bella_shoot_u2Tiles;
            _sprite_tiles_len = sprite_bella_shoot_u2TilesLen;

            _sprite_pal = sprite_bella_shoot_u2Pal;
            _sprite_pal_len = sprite_bella_shoot_u2PalLen;

            frame_count = 6;
            frame_size = 32*16;
            break;

        case(SpriteTypes::Type::QTIP):
            oam.size = SpriteSize_8x8;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_qtipTiles;
            _sprite_tiles_len = sprite_qtipTilesLen;

            _sprite_pal = sprite_qtipPal;
            _sprite_pal_len = sprite_qtipPalLen;
            break;

        case(SpriteTypes::Type::QTIP_RIFLE):
            oam.size = SpriteSize_32x32;
            _format = SpriteColorFormat_16Color;

            data = (u8*)sprite_qtip_rifleTiles;
            _sprite_tiles_len = sprite_qtip_rifleTilesLen;

            _sprite_pal = sprite_qtip_riflePal;
            _sprite_pal_len = sprite_qtip_riflePalLen;
            break;

        case(SpriteTypes::Type::SHADOW):
            oam.size = SpriteSize_32x8;
            _format = SpriteColorFormat_Bmp;
            oam.palette = new OAMPalette{0, SpriteColorFormat_Bmp, 0};

            data = (u8*)sprite_shadowBitmap;
            _sprite_tiles_len = sprite_shadowBitmapLen;

            // _sprite_pal = sprite_shadowPal;
            // _sprite_pal_len = sprite_shadowPalLen;
            break;
    }

    // allocate memory and copy the tiles into the tile buffer
    allocate_memory();
    dmaCopy(data, oam.pointer, _sprite_tiles_len);

    // if the palette is -1, it will allocate the sprite's palette into memory
    // if it is a number, then it indexes into the palette list instead of loading one in
    // (re-use palettes)
    if(_palette == -1)
    {
        for(int i = 0; i < 16; i++)
            if(PALETTE_SLOTS[i] == nullptr)
            {
                oam.palette = new OAMPalette{i, _format};
                break;
            }

        dmaCopy(_sprite_pal, &SPRITE_PALETTE[oam.palette->id*16], _sprite_pal_len);
        PALETTE_SLOTS[oam.palette->id] = oam.palette;
    }
    else if(_palette != -2)
        oam.palette = PALETTE_SLOTS[_palette];
}

void Sprite::allocate_memory()
{
    if(LOADED_TEX[type] == nullptr)
        LOADED_TEX[type] = oamAllocateGfx(&oamMain, oam.size, oam.palette->format);

    oam.pointer = LOADED_TEX[type];
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
        dmaCopy(offset, oam.pointer, frame_size);

        previous_frame = (int)frame;
    }
}

void Sprite::draw(Vector2 _camera)
{
    _animate();
    oam.draw(_camera);
}

void Sprite::draw_affine(Vector2 _camera, double _rotation, Vector2 _scale)
{
    _animate();
    oam.draw_affine(_camera, _rotation, _scale);
}

void Sprite::destroy(bool _global)
{
    if(_global)
    {
        oamFreeGfx(&oamMain, oam.pointer);
        LOADED_TEX[type] = nullptr;
        oam.pointer = nullptr;
    }

    oam.destroy();
    free(this);
}

void Sprite::_display()
{
    for(int i = 0; i < SPRITE_COUNT; i++)
    {
        if(OAM_SLOTS[i] != nullptr)
        {
            OAMObject* _oam = OAM_SLOTS[i];

            bool _mosaic = _oam->mosaic.cheap_length();

            oamSet(&oamMain, _oam->oam_id, 
            (int)(_oam->position.x - _oam->camera_offset.x), (int)(_oam->position.y - _oam->camera_offset.y), 
            _oam->priority, _oam->palette->id, _oam->size, _oam->palette->format, 
            _oam->pointer, _oam->affine_id, false, _oam->hide, _oam->flip_h, _oam->flip_v, _mosaic);

            if(_mosaic)
            {
                oamSetMosaicEnabled(&oamMain, _oam->oam_id, true);
                oamSetMosaic((int)min(15, _oam->mosaic.x), (int)min(15, _oam->mosaic.y));
            }
        }
    }

    for(int i = 0; i < SPRITE_COUNT; i++)
    {
        if(OAM_SLOTS[i] == nullptr)
        {
            oamClearSprite(&oamMain, i);
            continue;
        }

        if(!OAM_SLOTS[i]->static_slot)
        {
            OAM_SLOTS[i]->oam_id = -1;
            OAM_SLOTS[i] = nullptr;
        }
    }
}