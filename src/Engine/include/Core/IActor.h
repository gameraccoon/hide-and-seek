#pragma once

#include <string>

#include "Core/Vector2D.h"
#include "Core/Rotator.h"
#include "Structures/BoundingBox.h"
#include "Structures/Hull.h"
#include "Core/ActorComponent.h"

/** Types of an actors */
enum class ActorType
{
	Ghost,		// don't have collision, may not be shown
	Static,		// static objects like wall			// added in collisions list
	Dynamic,	// movable objects like barrel		// added in collisions list and check collisions only on moving
	Living,		// player or NPC					// added in collisions list and check collisions always
	Bullet,		// basic bullets					// check collisions all life cycle but not be added in collisions list
	Special,	// doors, rockets, turrels, etc.
	Light
};

/**
 * Interface for class Actor
 */
class IActor
{
public:
	using Ptr = std::unique_ptr<IActor>;

public:
	virtual ~IActor() = default;
	/** Process moving and other actions of the Actor */
	virtual void update(float deltatime) = 0;
	/** Say to actor, that it mast be destroyed now */
	virtual void destroy() = 0;
	/** Is actor wait to be automatically destroyed? */
	virtual bool isWaitDestruction() const = 0;
	/** Get actor type */
	virtual ActorType getType() const = 0;
	/** Take some damage to the actor */
	virtual void hit(IActor *instigator, float damageValue, Vector2D impulse) = 0;
	/** Returns the class identificator of this actor's class */
	virtual std::string getClassID() const = 0;
	/** Returns the specific identificator of current object */
	virtual std::string getActorId() const = 0;

	/** Add a new component to this actor */
	virtual void addComponent(const ActorComponent::Ptr& component) = 0;
	/** Get all the components of the class */
	virtual std::vector<ActorComponent::Ptr>& getAllComponents() = 0;
	template<typename T, typename... Args>
	std::shared_ptr<T> makeAndAddComponent(Args&&... args)
	{
		std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(args)...);
		addComponent(component);
		return component;
	}
	/** Get all the components of the specified type (highly not optimal for now) */
	template<typename T>
	std::vector<std::shared_ptr<T>> getComponents()
	{
		std::vector<std::shared_ptr<T>> resultComponents;
		auto& allComponents = getAllComponents();
		for (auto& component : allComponents)
		{
			std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(component);
			if (castedComponent != nullptr)
			{
				resultComponents.push_back(castedComponent);
			}
		}
		return resultComponents;
	}

	/** Get all the components of the specified type (highly not optimal for now) */
	template<typename T>
	std::vector<std::shared_ptr<const T>> getComponents() const
	{
		return const_cast<IActor*>(this)->getComponents<const T>();
	}

	/** Get the first components of the specified type (highly not optimal for now) */
	template<typename T>
	std::shared_ptr<T> getSingleComponent()
	{
		std::vector<std::shared_ptr<T>> resultComponents;
		auto& allComponents = getAllComponents();
		for (auto& component : allComponents)
		{
			std::shared_ptr<T> castedComponent = std::dynamic_pointer_cast<T>(component);
			if (castedComponent != nullptr)
			{
				return castedComponent;
			}
		}
		return std::shared_ptr<T>();
	}

	/** Get the first component of the specified type (highly not optimal for now) */
	template<typename T>
	std::shared_ptr<const T> getSingleComponent() const
	{
		return const_cast<IActor*>(this)->getSingleComponent<const T>();
	}
};
