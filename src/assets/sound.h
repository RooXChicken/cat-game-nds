#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <stdlib.h>
#include <maxmod9.h>

#include "../build/soundbank.h"
#include "../build/soundbank_bin.h"

struct Sound
{
    uint32_t type;
    uint16_t handle;

    double rate = 1.0;
    double volume = 1.0;
    
    double pan = 0.5;
    mm_sound_effect _effect;

    Sound(uint32_t _type);
    void _init();

    void play(bool _loop);
    void destroy();
};

#endif