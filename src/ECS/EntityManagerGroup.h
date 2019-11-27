#pragma once

#include <vector>

#include "ECS/EntityManager.h"
#include "ECS/EntityView.h"

class EntityManagerGroup
{
public:
	EntityManagerGroup(std::vector<EntityManager*>& managers);

	template<typename FirstComponent, typename... Components>
	void getComponents(std::vector<std::tuple<FirstComponent*, Components*...>>& inOutComponents)
	{
		for (EntityManager* manager : mManagers)
		{
			manager->getComponents<FirstComponent, Components...>(inOutComponents);
		}
	}

	template<typename FirstComponent, typename... Components, typename FunctionType>
	void forEachComponentSet(FunctionType processor)
	{
		for (EntityManager* manager : mManagers)
		{
			manager->forEachComponentSet<FirstComponent, Components...>(processor);
		}
	}

	template<typename FirstComponent, typename... Components, typename FunctionType>
	void forEachComponentSetWithEntity(FunctionType processor)
	{
		for (EntityManager* manager : mManagers)
		{
			manager->forEachComponentSetWithEntity<FirstComponent, Components...>([manager, &processor](Entity entity, FirstComponent* first, Components*... components)
			{
				processor(EntityView(entity, *manager), first, components...);
			});
		}
	}

	void executeScheduledActions();

private:
	std::vector<EntityManager*> mManagers;
};
