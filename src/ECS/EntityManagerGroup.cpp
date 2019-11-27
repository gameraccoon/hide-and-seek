#include "ECS/EntityManagerGroup.h"

#include "Base/Debug/Assert.h"

EntityManagerGroup::EntityManagerGroup(std::vector<EntityManager*>& managers)
	: mManagers(managers)
{
}

void EntityManagerGroup::executeScheduledActions()
{
	for (EntityManager* manager : mManagers)
	{
		manager->executeScheduledActions();
	}
}
