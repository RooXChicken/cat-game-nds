
//{{BLOCK(sprite_room)

//======================================================================
//
//	sprite_room, 640x648@8, 
//	+ palette 256 entries, not compressed
//	+ 96 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 80x81 
//	Total size: 512 + 6144 + 12960 = 19616
//
//	Time-stamp: 2025-01-20, 01:05:45
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SPRITE_ROOM_H
#define GRIT_SPRITE_ROOM_H

#define sprite_roomTilesLen 6144
extern const unsigned int sprite_roomTiles[1536];

#define sprite_roomMapLen 12960
extern const unsigned short sprite_roomMap[6480];

#define sprite_roomPalLen 512
extern const unsigned short sprite_roomPal[256];

#endif // GRIT_SPRITE_ROOM_H

//}}BLOCK(sprite_room)
