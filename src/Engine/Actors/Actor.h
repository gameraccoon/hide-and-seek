#ifndef ACTOR_H
#define ACTOR_H

#include <vector>

#include "../Core/Vector2D.h"
#include "../Core/IActor.h"
#include "../Core/World.h"
#include "../Core/Rotator.h"
#include "../Structures/BoundingBox.h"
#include "../Structures/Hull.h"

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
	void setLocation(const Vector2D& newLocations);
	/** Get actor's world location */
	Vector2D getLocation();
	/** Set actor's rotation */
	void setRotation(const Rotator& newRotation);
	/** Get actor's rotation */
	Rotator getRotation();
	/** Set actor's scale */
	void setScale(const Vector2D& newScale);
	/** Get actor's scale */
	Vector2D getScale();
	/** Process moving and other actions of the Actor */
	void update(float deltatime);
	/** Say to actor, that it mast be destroyed now */
	void destroy();
	/** Is actor wait to be automatically destroyed? */
	bool isWaitDestruction();
	/** Get actor type */
	EActorType getType();
	/** Get axis-aligned bounding box */
	BoundingBox getBoundingBox();
	/** Get actor's hull */
	Hull* getHull();
	/** Returns class identificator of this actor */
	std::string getClassID();
	/** Take some damage to the actor */
	virtual void takeDamage(float damageValue, Vector2D impulse) = 0;
protected:
	/** */
	virtual void updateCollision() = 0;
	/** Pointer to the owner World */
	World* ownerWorld;
	/** Type of the actor */
	EActorType type;
	/** Location of the actor in the world */
	Vector2D location;
	Vector2D scale;
	Vector2D originalSize;
	Vector2D calculatedSize;
	/** Angle between world's x-axis and actor's x-axis */
	Rotator direction;
	/** Bounding box */
	BoundingBox colideBox;
	/** Geometry for physics and shadows calculation */
	Hull geometry;
	/** Specific class identificator */
	std::string classID;
	/** Is actor wait automatically destruction? */
	bool bWaitDestruction;
	/** Time that actor live */
	float lifetime;
};

#endif
