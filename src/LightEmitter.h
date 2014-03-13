#ifndef LIGHTEMITTER_H
#define LIGHTEMITTER_H

#include "actor.h"

class LightEmitter : public Actor
{
public:
	LightEmitter(World *ownerWorld, Vector2D location);
	~LightEmitter(void);
	void update(float deltatime);
	virtual void takeDamage(float damageValue,Vector2D impulse);
protected:
	void updateCollision();
	float brightness;
	long color; // 0xAARRGGBB
};

#endif
