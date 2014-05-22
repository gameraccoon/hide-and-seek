#ifndef SOUND_VOLUME_H
#define SOUND_VOLUME_H

#include "../Actors/Actor.h"

class SoundVolume : public Actor
{
public:
	SoundVolume(World *world, Vector2D location, float loudness);
	~SoundVolume(void);

	float getLoudness();

protected:
	virtual void updateCollision() override;
private:
	float loudness;
};

#endif
