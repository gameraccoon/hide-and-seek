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
	Actor(World *world, Vector2D location, Rotator rotation);
	virtual ~Actor(void);
	
	/** Set new location of the actor in the World */
	virtual void setLocation(const Vector2D& newLocations) override final;
	/** Get actor's world location */
	virtual Vector2D getLocation() override final;
	/** Set actor's rotation */
	virtual void setRotation(const Rotator& newRotation) override final;
	/** Get actor's rotation */
	virtual Rotator getRotation() override final;
	/** Set actor's scale */
	virtual void setScale(const Vector2D& newScale) override final;
	/** Get actor's scale */
	virtual Vector2D getScale() override final;
	/** Process moving and other actions of the Actor */
	virtual void update(float deltatime) override;
	/** Say to actor, that it mast be destroyed now */
	virtual void destroy() override final;
	/** Is actor wait to be automatically destroyed? */
	virtual bool isWaitDestruction() override final;
	/** Get actor type */
	virtual ActorType getType() override final;
	/** Get axis-aligned bounding box */
	virtual BoundingBox getBoundingBox() override final;
	/** Get actor's hull */
	virtual Hull* getHull() override final;
	/** Returns the class identificator of this actor's class */
	virtual std::string getClassID() override final;
	/** Returns the specific identificator of current object */
	virtual std::string getActorId() override final;
	/** Take some damage to the actor */
	virtual void hit(IActor *instigator, float damageValue, Vector2D impulse) override;
protected:
	/** */
	virtual void updateCollision() = 0;
	/** Update classId and actorId */
	virtual void updateActorId(std::string classId);
	/** Pointer to the owner World */
	World* ownerWorld;
	/** Type of the actor */
	ActorType type;
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
	/** Actor's identificator */
	std::string actorId;
	/** Is actor wait automatically destruction? */
	bool bWaitDestruction;
	/** Time that actor live */
	float lifetime;
};

#endif
