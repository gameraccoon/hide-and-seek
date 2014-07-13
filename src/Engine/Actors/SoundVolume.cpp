#include "SoundVolume.h"


SoundVolume::SoundVolume(World *world, Vector2D location, float loudness) :
	Actor(world, location, 0.f)
{
	this->loudness = loudness;

	this->updateActorId("SoundVolume");
}

SoundVolume::~SoundVolume(void)
{
}
