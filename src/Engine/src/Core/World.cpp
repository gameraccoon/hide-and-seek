#include "Core/World.h"

#include <algorithm>
#include <random>

World::~World()
{
}

Entity World::addEntity()
{
	// ToDo: use generators
	Entity::EntityID id = static_cast<Entity::EntityID>(std::rand());
	mEntityIndexMap.emplace(id, mMaxEntityIndex++);
	return Entity(id);
}

void World::removeEntity(Entity entity)
{
	auto entityIdxItr = mEntityIndexMap.find(entity.getID());
	if (entityIdxItr == mEntityIndexMap.end())
	{
		return;
	}
	EntityIndex oldEntityIdx = entityIdxItr->second;

	for (auto& componentVector : mComponents)
	{
		// if the vector containts deleted entity
		if (componentVector.second.size() > oldEntityIdx)
		{
			// if the vector containts the last entity
			if (componentVector.second.size() < mMaxEntityIndex - 1)
			{
				// swap with the last and drop it
				std::swap(componentVector.second[oldEntityIdx], componentVector.second[mMaxEntityIndex - 1]);
				delete componentVector.second[mMaxEntityIndex - 1];
				componentVector.second.pop_back();
			}
			else
			{
				// emulate swapping with nonexsistent nullptr Component
				delete componentVector.second[oldEntityIdx];
				componentVector.second[oldEntityIdx] = nullptr;
			}
		}
	}

	--mMaxEntityIndex;

	mEntityIndexMap.erase(entity.getID());

	// relink maps
	Entity::EntityID entityID = mIndexEntityMap[mMaxEntityIndex];
	mEntityIndexMap[entityID] = oldEntityIdx;
	mIndexEntityMap[oldEntityIdx] = entityID;
	mIndexEntityMap.erase(mMaxEntityIndex);
}

void World::addPathPoint(PathPoint::Ptr& pathPoint)
{
	mNavigationMap.emplace_back(pathPoint.release());
}
