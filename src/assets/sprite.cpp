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

#include <../build/sprite_banana_bread.h>
#include <../build/sprite_boba_tea.h>
#include <../build/sprite_bread.h>
#include <../build/sprite_chalk.h>
#include <../build/sprite_cinnamon.h>
#include <../build/sprite_clothing_tag.h>
#include <../build/sprite_coffee.h>
#include <../build/sprite_lucky_bag.h>
#include <../build/sprite_mochi.h>
#include <../build/sprite_new_world.h>
#include <../build/sprite_peanut_butter_apple.h>
#include <../build/sprite_pineapple.h>
#include <../build/sprite_polaroid.h>
#include <../build/sprite_worn_toy_cat.h>
// #include <../build/sprite_shadow.h>

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
    oam = OAMObject{};
    // oam.palette = new OAMPalette{};

    if(_oam_id != -1)
    {
        oam.static_slot = true;
        oam.oam_id = _oam_id;
    }

    SpriteData _data = {};
    _get_sprite_data(_type, &_data);

    data = _data.data;
    oam.size = _data.size;
    
    frame_count = _data.frame_count;
    frame_size = _data.frame_size;

    // allocate memory and copy the tiles into the tile buffer
    allocate_memory(_data.format);
    dmaCopy(data, oam.pointer, _data.tiles_length);

    // if the palette is -1, it will allocate the sprite's palette into memory
    // if it is a number, then it indexes into the palette list instead of loading one in
    // (re-use palettes)
    if(_palette == -1)
    {
        for(int i = 0; i < 16; i++)
            if(PALETTE_SLOTS[i] == nullptr)
            {
                oam.palette = new OAMPalette{i, SpriteColorFormat_16Color};
                break;
            }

        dmaCopy(_data.palette, &SPRITE_PALETTE[oam.palette->id*16], _data.palette_length);
        PALETTE_SLOTS[oam.palette->id] = oam.palette;
    }
    else if(_palette != -2)
        oam.palette = PALETTE_SLOTS[_palette];
}

void Sprite::load_palette(SpriteTypes::Type _type, int _index)
{
    SpriteData _data = {};
    _get_sprite_data(_type, &_data);
    OAMPalette* _palette = new OAMPalette{_index, _data.format};

    dmaCopy(_data.palette, &SPRITE_PALETTE[_palette->id*16], _data.palette_length);
    PALETTE_SLOTS[_palette->id] = _palette;
}

void Sprite::allocate_memory(SpriteColorFormat _format)
{
    if(LOADED_TEX[type] == nullptr)
        LOADED_TEX[type] = oamAllocateGfx(&oamMain, oam.size, _format);

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
            _oam->pointer, _oam->affine_id, false, _oam->hide, _oam->flip_h, _oam->flip_v, false);

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

void Sprite::_get_sprite_data(SpriteTypes::Type _type, SpriteData* _data)
{
    // depending on the sprite type, load in the relevant data
    // (hard-coded for now. maybe will be dynamic if i can figure it out)
    switch(_type)
    {
        case(SpriteTypes::Type::KITA):
            _data->size = SpriteSize_64x64;
            _data->format = SpriteColorFormat_256Color;
            
            _data->data = (u8*)sprite_kitaTiles;
            _data->tiles_length = sprite_kitaTilesLen;

            _data->palette = sprite_kitaPal;
            _data->palette_length = sprite_kitaPalLen;
            break;

        case(SpriteTypes::Type::BELLA_WALK):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_bella_walkTiles;
            _data->tiles_length = sprite_bella_walkTilesLen;

            _data->palette = sprite_bella_walkPal;
            _data->palette_length = sprite_bella_walkPalLen;

            _data->frame_count = 6;
            _data->frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_WALK_ARMS):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_bella_walk_armsTiles;
            _data->tiles_length = sprite_bella_walk_armsTilesLen;

            _data->palette = sprite_bella_walk_armsPal;
            _data->palette_length = sprite_bella_walk_armsPalLen;

            _data->frame_count = 6;
            _data->frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_IDLE):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_bella_idleTiles;
            _data->tiles_length = sprite_bella_idleTilesLen;

            _data->palette = sprite_bella_idlePal;
            _data->palette_length = sprite_bella_idlePalLen;

            _data->frame_count = 6;
            _data->frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_IDLE_ARMS):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_bella_idle_armsTiles;
            _data->tiles_length = sprite_bella_idle_armsTilesLen;

            _data->palette = sprite_bella_idle_armsPal;
            _data->palette_length = sprite_bella_idle_armsPalLen;

            _data->frame_count = 6;
            _data->frame_size = 32*16;
            break;

        case(SpriteTypes::Type::CROSSHAIR):
            _data->size = SpriteSize_8x8;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_crosshairTiles;
            _data->tiles_length = sprite_crosshairTilesLen;

            _data->palette = sprite_crosshairPal;
            _data->palette_length = sprite_crosshairPalLen;
            break;

        case(SpriteTypes::Type::TREAT_PISTOL):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_treat_pistolTiles;
            _data->tiles_length = sprite_treat_pistolTilesLen;

            _data->palette = sprite_treat_pistolPal;
            _data->palette_length = sprite_treat_pistolPalLen;
            break;

        case(SpriteTypes::Type::CATNIP_CANON):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_catnip_launcherTiles;
            _data->tiles_length = sprite_catnip_launcherTilesLen;

            _data->palette = sprite_catnip_launcherPal;
            _data->palette_length = sprite_catnip_launcherPalLen;
            break;

        case(SpriteTypes::Type::CAT_TREAT):
            _data->size = SpriteSize_8x8;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_cat_treatTiles;
            _data->tiles_length = sprite_cat_treatTilesLen;

            _data->palette = sprite_cat_treatPal;
            _data->palette_length = sprite_cat_treatPalLen;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_D):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_bella_shoot_dTiles;
            _data->tiles_length = sprite_bella_shoot_dTilesLen;

            _data->palette = sprite_bella_shoot_dPal;
            _data->palette_length = sprite_bella_shoot_dPalLen;

            _data->frame_count = 6;
            _data->frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_D2):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_bella_shoot_d2Tiles;
            _data->tiles_length = sprite_bella_shoot_d2TilesLen;

            _data->palette = sprite_bella_shoot_d2Pal;
            _data->palette_length = sprite_bella_shoot_d2PalLen;

            _data->frame_count = 6;
            _data->frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_M):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_bella_shoot_mTiles;
            _data->tiles_length = sprite_bella_shoot_mTilesLen;

            _data->palette = sprite_bella_shoot_mPal;
            _data->palette_length = sprite_bella_shoot_mPalLen;

            _data->frame_count = 6;
            _data->frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_M2):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_bella_shoot_m2Tiles;
            _data->tiles_length = sprite_bella_shoot_m2TilesLen;

            _data->palette = sprite_bella_shoot_m2Pal;
            _data->palette_length = sprite_bella_shoot_m2PalLen;

            _data->frame_count = 6;
            _data->frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_U):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_bella_shoot_uTiles;
            _data->tiles_length = sprite_bella_shoot_uTilesLen;

            _data->palette = sprite_bella_shoot_uPal;
            _data->palette_length = sprite_bella_shoot_uPalLen;

            _data->frame_count = 6;
            _data->frame_size = 32*16;
            break;

        case(SpriteTypes::Type::BELLA_SHOOT_U2):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_bella_shoot_u2Tiles;
            _data->tiles_length = sprite_bella_shoot_u2TilesLen;

            _data->palette = sprite_bella_shoot_u2Pal;
            _data->palette_length = sprite_bella_shoot_u2PalLen;

            _data->frame_count = 6;
            _data->frame_size = 32*16;
            break;

        case(SpriteTypes::Type::QTIP):
            _data->size = SpriteSize_8x8;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_qtipTiles;
            _data->tiles_length = sprite_qtipTilesLen;

            _data->palette = sprite_qtipPal;
            _data->palette_length = sprite_qtipPalLen;
            break;

        case(SpriteTypes::Type::QTIP_RIFLE):
            _data->size = SpriteSize_32x32;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_qtip_rifleTiles;
            _data->tiles_length = sprite_qtip_rifleTilesLen;

            _data->palette = sprite_qtip_riflePal;
            _data->palette_length = sprite_qtip_riflePalLen;
            break;

        case(SpriteTypes::Type::BANANA_BREAD):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_banana_breadTiles;
            _data->tiles_length = sprite_banana_breadTilesLen;

            _data->palette = sprite_banana_breadPal;
            _data->palette_length = sprite_banana_breadPalLen;
            break;

        case(SpriteTypes::Type::BOBA_TEA):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_boba_teaTiles;
            _data->tiles_length = sprite_boba_teaTilesLen;

            _data->palette = sprite_boba_teaPal;
            _data->palette_length = sprite_boba_teaPalLen;
            break;

        case(SpriteTypes::Type::BREAD):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_breadTiles;
            _data->tiles_length = sprite_breadTilesLen;

            _data->palette = sprite_breadPal;
            _data->palette_length = sprite_breadPalLen;
            break;

        case(SpriteTypes::Type::CHALK):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_chalkTiles;
            _data->tiles_length = sprite_chalkTilesLen;

            _data->palette = sprite_chalkPal;
            _data->palette_length = sprite_chalkPalLen;
            break;

        case(SpriteTypes::Type::CINNAMON):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_cinnamonTiles;
            _data->tiles_length = sprite_cinnamonTilesLen;

            _data->palette = sprite_cinnamonPal;
            _data->palette_length = sprite_cinnamonPalLen;
            break;

        case(SpriteTypes::Type::CLOTHING_TAG):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_clothing_tagTiles;
            _data->tiles_length = sprite_clothing_tagTilesLen;

            _data->palette = sprite_clothing_tagPal;
            _data->palette_length = sprite_clothing_tagPalLen;
            break;

        case(SpriteTypes::Type::COFFEE):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_coffeeTiles;
            _data->tiles_length = sprite_coffeeTilesLen;

            _data->palette = sprite_coffeePal;
            _data->palette_length = sprite_coffeePalLen;
            break;

        case(SpriteTypes::Type::LUCKY_BAG):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_lucky_bagTiles;
            _data->tiles_length = sprite_lucky_bagTilesLen;

            _data->palette = sprite_lucky_bagPal;
            _data->palette_length = sprite_lucky_bagPalLen;
            break;

        case(SpriteTypes::Type::NEW_WORLD):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_new_worldTiles;
            _data->tiles_length = sprite_new_worldTilesLen;

            _data->palette = sprite_new_worldPal;
            _data->palette_length = sprite_new_worldPalLen;
            break;

        case(SpriteTypes::Type::PINEAPPLE):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_pineappleTiles;
            _data->tiles_length = sprite_pineappleTilesLen;

            _data->palette = sprite_pineapplePal;
            _data->palette_length = sprite_pineapplePalLen;
            break;

        case(SpriteTypes::Type::MOCHI):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_mochiTiles;
            _data->tiles_length = sprite_mochiTilesLen;

            _data->palette = sprite_mochiPal;
            _data->palette_length = sprite_mochiPalLen;
            break;

        case(SpriteTypes::Type::PEANUT_BUTTER_APPLE):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_peanut_butter_appleTiles;
            _data->tiles_length = sprite_peanut_butter_appleTilesLen;

            _data->palette = sprite_peanut_butter_applePal;
            _data->palette_length = sprite_peanut_butter_applePalLen;
            break;

        case(SpriteTypes::Type::POLAROID):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_polaroidTiles;
            _data->tiles_length = sprite_polaroidTilesLen;

            _data->palette = sprite_polaroidPal;
            _data->palette_length = sprite_polaroidPalLen;
            break;

        case(SpriteTypes::Type::WORN_TOY_CAT):
            _data->size = SpriteSize_16x16;
            _data->format = SpriteColorFormat_16Color;

            _data->data = (u8*)sprite_worn_toy_catTiles;
            _data->tiles_length = sprite_worn_toy_catTilesLen;

            _data->palette = sprite_worn_toy_catPal;
            _data->palette_length = sprite_worn_toy_catPalLen;
            break;
    }
}