#include "Actors/SoundVolume.h"


SoundVolume::SoundVolume(World *world, float loudness)
	: Actor(world)
    , mLoudness(loudness)
{
	updateActorId("SoundVolume");
}

SoundVolume::~SoundVolume()
{
}

float SoundVolume::getLoudness() const
{
	return mLoudness;
}
