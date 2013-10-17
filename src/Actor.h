#ifndef ACTOR_H
#define ACTOR_H

#include "../src/Globals.h"
#include "../src/Vector2D.h"

/**
 * The base class for all actors which may be placed in the World.
 *
 * Abstract
 */
class Actor
{
public:
	Actor(Vector2D location);
	~Actor(void);
	
	/** Set new location of the actor in the World */
	void SetLocation(Vector2D newLocations);
	/** Get actor's world location */
	Vector2D GetLocation();
	/** Process moving and other actions of the Actor */
	virtual void Update(float deltaTime) = 0;
	/** Render the actor in the current location */
	virtual void Render(Vector2D shift, Rotator angle) = 0;
protected:
	/** Location of the actor in the world */
	Vector2D Location;
	/** Angle between world's x-axis and actor's x-axis */
	Rotator Direction;
	/** Sprite of the actor */
	hgeSprite *Sprite;
};

#endif