#include "HAL/Audio/Music.h"

#include "sdl/SDL_mixer.h"

#include "HAL/Internal/SdlSurface.h"
#include "HAL/Base/Engine.h"

namespace Audio
{
	Music::Music(const std::string& path)
		: mMusic(Mix_LoadMUS(path.c_str()))
	{
	}

	Music::~Music()
	{
		Mix_FreeMusic(mMusic);
	}

	Mix_Music* Music::getRawMusic() const
	{
		return mMusic;
	}

	bool Music::isValid() const
	{
		return mMusic != nullptr;
	}
}
