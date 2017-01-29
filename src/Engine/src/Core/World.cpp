#include "Core/World.h"

#include <algorithm>

World::~World()
{
}

void World::addActor(IActor::Ptr& actor)
{
	mAllActors.emplace_back(actor.release());
}

void World::update(float deltatime)
{
	for (auto& actor : mAllActors)
	{
		if (!actor->isWaitDestruction())
		{
			actor->update(deltatime);
		}
	}

	cleanDestroyedActors();
}

void World::addPathPoint(PathPoint::Ptr& pathPoint)
{
	mNavigationMap.emplace_back(pathPoint.release());
}

void World::cleanDestroyedActors()
{
	auto i = mAllActors.begin(), iEnd = mAllActors.end();
	while (i != iEnd)
	{
		IActor* currentActor = i->get();

		if (currentActor->isWaitDestruction())
		{
			i = mAllActors.erase(i);
		}
		else
		{
			i++;
		}
	}
}

IActor* World::getActorById(const std::string& id)
{
	for (const auto &actor : mAllActors)
	{
		if (id == actor->getActorId())
		{
			return actor.get();
		}
	}

	return nullptr;
}

std::vector<IActor::Ptr>& World::getAllActors()
{
	return mAllActors;
}

const std::vector<IActor::Ptr>& World::getAllActors() const
{
	return mAllActors;
}

const std::vector<PathPoint::Ptr>& World::getNavigationMap() const
{
	return mNavigationMap;
}
