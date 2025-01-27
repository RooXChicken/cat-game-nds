#include <../assets/sound.h>

Sound::Sound(uint32_t _type)
{
    type = _type;
    _init();
}

void Sound::_init()
{
    mmLoadEffect(type);
    _effect =
	{
		{type}, // id
		(uint16_t)(rate * (1<<10)), // rate ??
		handle, // handle
		(uint8_t)(255*volume), // volume
		(uint8_t)(255*pan) // panning
	};
}

void Sound::play(bool _loop)
{
    _effect.rate = (uint16_t)(rate * (1<<10));
    _effect.volume = (uint8_t)(255*volume);
    _effect.panning = (uint8_t)(255*pan);

    mmEffectEx(&_effect);
}

void Sound::destroy()
{
    mmEffectRelease(handle);
    free(this);
}