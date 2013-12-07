#ifndef ACTOR_H
#define ACTOR_H

#include <vector>

#include "../src/Globals.h"
#include "../src/Vector2D.h"
#include "../src/IActor.h"
#include "../src/World.h"


/**
 * The base class for all actors which may be placed in the World.
 *
 * Abstract
 */
class Actor : public IActor
{
public:
	Actor(World *ownerWorld, Vector2D location);
	virtual ~Actor(void);
	
	/** Set new location of the actor in the World */
	void SetLocation(Vector2D newLocations);
	/** Get actor's world location */
	Vector2D GetLocation();
	/** Process moving and other actions of the Actor */
	virtual void Update(float deltaTime) = 0;
	/** Render the actor in the current location */
	virtual void Render(Vector2D shift, Rotator angle) = 0;
	/** Get actor type */
	EActorType GetType();
	/** Get axis-aligned bounding box */
	BoundingBox GetBoundingBox();
	/** Get actor's hull */
	Hull* GetHull();
protected:
	/** */
	virtual void UpdateCollision() = 0;
	/** Pointer of the HGE subsystem */
	HGE *Hge;
	/** Pointer to the owner World */
	World* OwnerWorld;
	/** Type of the actor */
	EActorType Type;
	/** Location of the actor in the world */
	Vector2D Location;
	/** Angle between world's x-axis and actor's x-axis */
	Rotator Direction;
	/** Sprite of the actor */
	hgeSprite *Sprite;
	/** Bounding box */
	BoundingBox ColideBox;
	/** Geometry for physics and shadows calculation */
	Hull Geometry;
};

#endif