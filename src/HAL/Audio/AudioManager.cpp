#include "HAL/Audio/AudioManager.h"

#include <cmath>

#include "sdl/SDL_mixer.h"

#include "HAL/Audio/Sound.h"
#include "HAL/Audio/Music.h"

namespace Audio
{
	void AudioManager::PlaySound(const Sound& sound)
	{
		Mix_PlayChannel(-1, sound.getRawSound(), 0);
	}

	void AudioManager::SetSoundVolume(const Sound& sound, float volume)
	{
		Mix_VolumeChunk(sound.getRawSound(), static_cast<int>(std::ceil(volume * 128.0f)));
	}

	float AudioManager::GetSoundVolume(const Sound& sound)
	{
		return Mix_VolumeChunk(sound.getRawSound(), -1) / 128.0f;
	}

	void AudioManager::PlayMusic(const Music& music)
	{
		Mix_PlayMusic(music.getRawMusic(), -1);
	}

	void AudioManager::PauseMusic()
	{
		Mix_PauseMusic();
	}

	void AudioManager::ResumeMusic()
	{
		Mix_ResumeMusic();
	}

	void AudioManager::StopMusic()
	{
		Mix_HaltMusic();
	}

	bool AudioManager::IsMusicPlaying()
	{
		return Mix_PlayingMusic() != 0;
	}

	bool AudioManager::IsMusicPaused()
	{
		return Mix_PausedMusic() == 1;
	}

	void AudioManager::SetMusicVolume(float volume)
	{
		Mix_VolumeMusic(static_cast<int>(std::ceil(volume * 128.0f)));
	}

	float AudioManager::GetMusicVolume()
	{
		return Mix_VolumeMusic(-1) / 128.0f;
	}
}
