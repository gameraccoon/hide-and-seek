#include "SoundVolume.h"

#include <DebugMethods.h>

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
 	#define RELEASE
  	#define WARN(message)
  	#define WARN_IF(condition, message)
#endif


SoundVolume::SoundVolume(World *world, Vector2D location, float loudness) :
	Actor(world, location, 0.f)
{
	this->loudness = loudness;

	this->updateActorId("SoundVolume");
}

SoundVolume::~SoundVolume(void)
{
}

void SoundVolume::updateCollision()
{
}
