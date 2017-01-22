#include "Actors/SoundVolume.h"


SoundVolume::SoundVolume(World *world, Vector2D location, float loudness)
	: Actor(world, location, 0.f)
{
	loudness = loudness;

	updateActorId("SoundVolume");
}

SoundVolume::~SoundVolume()
{
}

float SoundVolume::getLoudness() const
{
	return mLoudness;
}
