#pragma once

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
	virtual ~Actor();
	
	/** Set new location of the actor in the World */
	virtual void setLocation(const Vector2D& newLocations) override final;
	/** Get actor's world location */
	virtual Vector2D getLocation() const override final;
	/** Set actor's rotation */
	virtual void setRotation(const Rotator& newRotation) override final;
	/** Get actor's rotation */
	virtual Rotator getRotation() const override final;
	/** Set actor's scale */
	virtual void setScale(const Vector2D& newScale) override final;
	/** Get actor's scale */
	virtual Vector2D getScale() const override final;
	/** Process moving and other actions of the Actor */
	virtual void update(float deltatime) override;
	/** Say to actor, that it mast be destroyed now */
	virtual void destroy() override final;
	/** Is actor wait to be automatically destroyed? */
	virtual bool isWaitDestruction() const override final;
	/** Get actor type */
	virtual ActorType getType() const override final;
	/** Get axis-aligned bounding box */
	virtual BoundingBox getBoundingBox() const override final;
	/** Get actor's hull */
	virtual const Hull* getGeometry() const override final;
	/** Returns the class identificator of this actor's class */
	virtual std::string getClassID() const override final;
	/** Returns the specific identificator of current object */
	virtual std::string getActorId() const override final;
	/** Take some damage to the actor */
	virtual void hit(IActor *instigator, float damageValue, Vector2D impulse) override;

protected:
	/* events */
	virtual void onUpdateLocation();
	virtual void onUpdateRotation();
	virtual void onUpdateSize();
	/** Update classId and actorId */
	virtual void updateActorId(std::string classId);
	
	float getLifetime() const;
	World* getOwnerWorld() const;
	Vector2D getCalculatedSize() const;
	void setType(ActorType newType);
	void setOriginalSize(Vector2D newOriginalSize);
	Vector2D getOriginalSize();
	void setGeometry(Hull newGeometry);
	void updateGeometry();
	void setClassId(std::string newClassId);
	void setActorId(std::string newActorId);

private:
	/** */
	void updateCollision();
	void setColideBox(BoundingBox newColideBox);
	/** Pointer to the owner World */
	World* mOwnerWorld;
	/** Type of the actor */
	ActorType mType;
	/** Location of the actor in the world */
	Vector2D mLocation;
	Vector2D mScale;
	Vector2D mOriginalSize;
	Vector2D mCalculatedSize;
	/** Angle between world's x-axis and actor's x-axis */
	Rotator mDirection;
	/** Bounding box */
	BoundingBox mColideBox;
	/** Geometry for physics and shadows calculation */
	Hull mInitialGeometry;
	/** Scaled and rotated geometry */
	Hull mGeometry;
	/** Specific class identificator */
	std::string mClassId;
	/** Actor's identificator */
	std::string mActorId;
	/** Is actor wait automatically destruction? */
	bool mIsWaitDestruction;
	/** Time that actor live */
	float mLifetime;

	/** Copy and assignment prohibited */
	Actor(const Actor&) = delete;
	void operator=(const Actor&) = delete;
};
