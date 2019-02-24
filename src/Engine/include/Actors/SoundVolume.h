#pragma once

#include "Core/Actor.h"

class SoundVolume : public Actor
{
public:
	SoundVolume(World *world, Vector2D location, float loudness);
	~SoundVolume() override;

	float getLoudness() const;

private:
	float mLoudness;
};
