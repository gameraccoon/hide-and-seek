#pragma once

#include <vector>

#include <Core/Vector2D.h>
#include <Core/IActor.h>
#include <Core/World.h>
#include <Core/Rotator.h>
#include <Structures/BoundingBox.h>
#include <Structures/Hull.h>


/**
 * The base class for all actors which may be placed in the World.
 *
 * Abstract
 */
class Actor : public IActor
{
public:
	using Ptr = std::unique_ptr<Actor>;

public:
	static Ptr Create(World *world);

	Actor(World *world);
	virtual ~Actor() override;
	/** Process moving and other actions of the Actor */
	virtual void update(float deltatime) override;
	/** Say to actor, that it mast be destroyed now */
	virtual void destroy() override final;
	/** Is actor wait to be automatically destroyed? */
	virtual bool isWaitDestruction() const override final;
	/** Get actor type */
	virtual ActorType getType() const override final;
	/** Returns the class identificator of this actor's class */
	virtual std::string getClassID() const override final;
	/** Returns the specific identificator of current object */
	virtual std::string getActorId() const override final;
	/** Take some damage to the actor */
	virtual void hit(IActor *instigator, float damageValue, Vector2D impulse) override;

	/** Add a new component to this actor */
	virtual void addComponent(const ActorComponent::Ptr& component) override;
	virtual std::vector<ActorComponent::Ptr>& getAllComponents() override;

protected:
	/** Update classId and actorId */
	virtual void updateActorId(std::string classId);
	
	float getLifetime() const;
	World* getOwnerWorld() const;
	void setType(ActorType newType);
	void setClassId(std::string newClassId);
	void setActorId(std::string newActorId);

private:
	/** Pointer to the owner World */
	World* mOwnerWorld;
	/** Type of the actor */
	ActorType mType;
	/** Specific class identificator */
	std::string mClassId;
	/** Actor's identificator */
	std::string mActorId;
	/** Is actor wait automatically destruction? */
	bool mIsWaitDestruction;
	/** Time that actor live */
	float mLifetime;
	/** All the components that attached to this actor */
	std::vector<ActorComponent::Ptr> mComponents;

	/** Copy and assignment prohibited */
	Actor(const Actor&) = delete;
	void operator=(const Actor&) = delete;
};
