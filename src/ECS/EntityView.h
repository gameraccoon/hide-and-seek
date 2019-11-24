#pragma once

#include "ECS/Entity.h"
#include "ECS/EntityManager.h"

/**
 * @brief Non-owning wrapper around entity amd its current entity manager
 */
class EntityView
{
public:
	EntityView(Entity entity, EntityManager& manager);

	template<typename ComponentType>
	bool hasComponent()
	{
		return mManager.doesEntityHaveComponent<ComponentType>(mEntity);
	}

	template<typename ComponentType>
	ComponentType* addComponent()
	{
		return mManager.addComponent<ComponentType>(mEntity);
	}

	template<typename ComponentType>
	void removeComponent()
	{
		mManager.removeComponent<ComponentType>(mEntity);
	}

	template<typename... Components>
	std::tuple<Components*...> getComponents()
	{
		return mManager.getEntityComponents<Components...>(mEntity);
	}

	Entity getEntity() const;

private:
	Entity mEntity;
	EntityManager& mManager;
};
