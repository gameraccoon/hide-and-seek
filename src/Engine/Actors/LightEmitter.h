#ifndef LIGHTEMITTER_H
#define LIGHTEMITTER_H

#include "Actor.h"

class LightEmitter : public Actor
{
public:
	LightEmitter(World *world, Vector2D location, Vector2D scale, Rotator rotation);
	~LightEmitter(void);
protected:
	virtual void updateCollision() override;
	float brightness;
	long color; // 0xAARRGGBB
};

#endif
