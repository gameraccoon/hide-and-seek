#ifndef LIGHTEMITTER_H
#define LIGHTEMITTER_H

#include "actor.h"

class LightEmitter :
	public Actor
{
public:
	LightEmitter(World *ownerWorld, Vector2D location);
	~LightEmitter(void);
	void Update(float deltaTime);
	/** Render the actor in the current location */
	void Render(Vector2D shift, Rotator angle);
protected:
	void UpdateCollision();
	float Brightness;
	long Color; // 0xAARRGGBB
};

#endif