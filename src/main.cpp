#define __NDS__
#define ARM9

#include <nds.h>
#include <maxmod9.h>
#include <stdio.h>
#include <math.h>

#include <../src/math/vector2.h>
#include <../src/entity/entity.h>
#include <../src/assets/sprite.h>
#include <../src/assets/sound.h>

#include <../build/sprite_room.h>

void init_video();
void init_backgrounds();
void init_audio();

void scroll_camera();
void display();

static Vector2 camera_position;
static Vector2 old_camera_position;

static Entity entities[128];

int room_bg = 0;
const int TILE_WIDTH = 8; // width of a tile in pixels
const int MAP_WIDTH = 640 / 8; // width of the big map in tiles
const int MAP_HEIGHT = 648 / 8; // heigh of the big map in tiles
const int BG_WIDTH = 256 / 8; // width of the hardware map in tiles
const int BG_HEIGHT = 256 / 8; // height of the hardware map in tiles
const int TILED_SCREEN_WIDTH = 256 / 8; // screen width in tiles
const int TILED_SCREEN_HEIGHT = 192 / 8; // screen height in tiles

int main(void)
{
	powerOn(POWER_ALL_2D);

	init_video();
	init_backgrounds();
	init_audio();

	Sprite bella = Sprite(SpriteType::BELLA, -1, -1);
	Sprite bella_arms = Sprite(SpriteType::BELLA_ARMS, -1, -1);
	bella.priority = 1;

	int t = 0;
	double move_speed = 0.5;
	double decel_speed = 0.2;
	double speed_cap = 2;

	Vector2 velocity = {0, 0};
	Vector2 move_dir = {0, 0};

	while(pmMainLoop())
	{
		scanKeys();

		move_dir = {0, 0};
		if(keysHeld() & KEY_LEFT)
			move_dir.x -= 1;
		if(keysHeld() & KEY_RIGHT)
			move_dir.x += 1;
		if(keysHeld() & KEY_UP)
			move_dir.y -= 1;
		if(keysHeld() & KEY_DOWN)
			move_dir.y += 1;

		move_dir = move_dir.normalize();
		velocity += move_dir * move_speed;

		bella.position += velocity;

		if(velocity.x < 0)
			bella.flip_h = true;
		else if(velocity.x > 0)
			bella.flip_h = false;

		bella.frame += velocity.length()/12;

		if(velocity.x > 0)
		{
			velocity.x -= decel_speed;

			if(velocity.x > speed_cap)
				velocity.x -= move_speed;

			if(velocity.x < 0)
				velocity.x = 0;
		}
		else if(velocity.x < 0)
		{
			velocity.x += decel_speed;

			if(velocity.x < -speed_cap)
				velocity.x += move_speed;

			if(velocity.x > 0)
				velocity.x = 0;
		}

		if(velocity.y > 0)
		{
			velocity.y -= decel_speed;

			if(velocity.y > speed_cap)
				velocity.y -= move_speed;

			if(velocity.y < 0)
				velocity.y = 0;
		}
		else if(velocity.y < 0)
		{
			velocity.y += decel_speed;

			if(velocity.y < -speed_cap)
				velocity.y += move_speed;

			if(velocity.y > 0)
				velocity.y = 0;
		}

		bella_arms.frame = bella.frame;
		bella_arms.position = bella.position;
		bella_arms.flip_h = bella.flip_h;

		camera_position.x = bella.position.x - (SCREEN_WIDTH/2);
		camera_position.y = bella.position.y - (SCREEN_HEIGHT/2);
		scroll_camera();

		bella.draw(camera_position);
		bella_arms.draw(camera_position);

		for(int i = 0; i < 128; i++)
		{
			if(entities[i].type != EntityType::EMPTY)
				entities[i].draw(camera_position);
		}

		display();
		t++;
	}

	return 0;
}

void init_video()
{
	// video mode for the main engine (top screen, in this case)
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D);

	// video mode for the sub engine (bottom screen for us)
	videoSetModeSub(MODE_0_2D | DISPLAY_BG0_ACTIVE | DISPLAY_SPR_ACTIVE | DISPLAY_SPR_1D);

	// initialize vram banks
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000); // used for background layer 0
	vramSetBankE(VRAM_E_MAIN_SPRITE); // used for all sprites (64KB, but enough for 1024 16 color sprites)

	// initialize the main OAM
	oamInit(&oamMain, SpriteMapping_1D_128, false);
}

void init_backgrounds()
{
	room_bg = bgInit(0, BgType_Text8bpp, BgSize_T_512x256, 0, 1);
	bgSetPriority(room_bg, 3);

	dmaCopy(sprite_roomTiles, bgGetGfxPtr(room_bg), sprite_roomTilesLen);
	dmaCopy(sprite_roomPal, BG_PALETTE, sprite_roomPalLen);
	
	u16* bgTileMap = (u16*)bgGetMapPtr(room_bg);
	
	for(int i = 0; i < TILED_SCREEN_HEIGHT; i++)
      	dmaCopy(&sprite_roomMap[i * MAP_WIDTH], &bgTileMap[i * BG_WIDTH], TILED_SCREEN_WIDTH * 2);
}

void init_audio()
{
	mmInitDefaultMem((mm_addr)soundbank_bin);
}

void scroll_camera()
{
	if(camera_position.x == old_camera_position.x && camera_position.y == old_camera_position.y)
		return;

	if(camera_position.x < 0)
		camera_position.x = 0;
	if(camera_position.y < 0)
		camera_position.y = 0;

	const int BORDER_WIDTH = MAP_WIDTH*8;
	const int BORDER_HEIGHT = MAP_HEIGHT*8;

	if(camera_position.x > BORDER_WIDTH - SCREEN_WIDTH)
		camera_position.x = BORDER_WIDTH - SCREEN_WIDTH;
	if(camera_position.y > BORDER_HEIGHT - SCREEN_HEIGHT)
		camera_position.y = BORDER_HEIGHT - SCREEN_HEIGHT;

	int offset_x = 0;
   	int offset_y = 0;

	u16* bgTileMap = (u16*)bgGetMapPtr(room_bg);
	u16* bgLeftHalf = bgTileMap;
	u16* bgRightHalf = bgTileMap + 32 * 32;

	bool move_horizontal = false;
	bool move_vertical = false;

	if(camera_position.x < old_camera_position.x) // move left
	{
		offset_x = (int)camera_position.x / 8 - 1;
		move_horizontal = true;
	}
	else if(camera_position.x > old_camera_position.x) // move left
	{
		offset_x = (int)camera_position.x / 8 + TILED_SCREEN_WIDTH;
		move_horizontal = true;
	}

	if(camera_position.y < old_camera_position.y) // move up
	{
		offset_y = (int)camera_position.y / 8 - 1;
		move_vertical = true;
	}
	else if(camera_position.y > old_camera_position.y) // move down
	{
		offset_y = (int)camera_position.y / 8 + TILED_SCREEN_HEIGHT;
		move_vertical = true;
	}

	if(move_horizontal)
	{
		u16* bgTemp = ((offset_x & 63) >= BG_WIDTH) ? bgRightHalf : bgLeftHalf;

		for(int iy = (int)camera_position.y / 8 - 1 ; iy < (int)camera_position.y / 8 + TILED_SCREEN_HEIGHT + 1; iy++)
			bgTemp[(offset_x & (BG_WIDTH - 1)) + (iy & (BG_HEIGHT - 1)) * 32] = sprite_roomMap[offset_x + iy * MAP_WIDTH];
	}

	if(move_vertical)
	{
		for(int ix = (int)camera_position.x / 8 - 1 ; ix < (int)camera_position.x / 8 + TILED_SCREEN_WIDTH + 1; ix++)
		{
			u16* bgTemp = ((ix & 63) >= BG_WIDTH) ? bgRightHalf : bgLeftHalf;
			bgTemp[(ix & (BG_WIDTH - 1)) + (offset_y & (BG_HEIGHT - 1))* 32] = sprite_roomMap[ix + offset_y * MAP_WIDTH];
		}
	}

	bgSetScroll(room_bg, (int)camera_position.x, (int)camera_position.y);
	bgUpdate();

	old_camera_position = camera_position;
}

void display()
{
	// 'vsync' :3
	swiWaitForVBlank();
	
	// update the modified OAM buffer
	oamUpdate(&oamMain);
}