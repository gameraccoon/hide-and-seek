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
	Actor(World *ownerWorld, Vector2D location, Rotator rotation);
	virtual ~Actor(void);
	
	/** Set new location of the actor in the World */
	void SetLocation(const Vector2D& newLocations);
	/** Get actor's world location */
	Vector2D GetLocation();
	/** Set actor's rotation */
	void SetRotation(const Rotator& newRotation);
	/** Get actor's rotation */
	Rotator GetRotation();
	/** Set actor's scale */
	void SetScale(const Vector2D& newScale);
	/** Get actor's scale */
	Vector2D GetScale();
	/** Process moving and other actions of the Actor */
	void Update(float deltaTime);
	/** Render the actor in the current location */
	void Render(Vector2D shift, Rotator angle);
	/** Get actor type */
	EActorType GetType();
	/** Get axis-aligned bounding box */
	BoundingBox GetBoundingBox();
	/** Get actor's hull */
	Hull* GetHull();
	/** Returns class identificator of this actor */
	std::string GetClassID();
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
	Vector2D Scale;
	Vector2D OriginalSize;
	Vector2D CalculatedSize;
	/** Angle between world's x-axis and actor's x-axis */
	Rotator Direction;
	/** Sprite of the actor */
	hgeSprite *Sprite;
	/** Bounding box */
	BoundingBox ColideBox;
	/** Geometry for physics and shadows calculation */
	Hull Geometry;
	/** Specific class identificator */
	std::string ClassID;
};

#endif