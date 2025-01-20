#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <maxmod9.h>

struct Sound
{
    uint32_t type;
    uint16_t handle;

    double rate = 1.0;
    double volume = 1.0;
    
    double pan = 0.5;
    mm_sound_effect _effect;

    static Sound load(uint32_t _type);
    void _init();

    void play(bool _loop);
};

#endif