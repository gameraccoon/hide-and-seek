#include "World.h"

World::World()
{
}

World::~World()
{
	auto i = mAllActors.begin(), iEnd = mAllActors.end();

	while (i != iEnd)
	{
		IActor* actorToDelete = (*i);
		i++;
		
		mAllActors.erase(actorToDelete);

		delete (actorToDelete);
	}

	mAllActors.clear();
	removeAllPathPoints();
}

void World::spawnActor(IActor* actor)
{
	mAllActors.insert(actor);
}

void World::removeActor(IActor* actor)
{
	mAllActors.erase(actor);
}

void World::update(float deltatime)
{
	for (auto actor : mAllActors)
	{
		if (!actor->isWaitDestruction())
		{
			actor->update(deltatime);
		}
	}

	cleanDestroyedActors();
}

void World::addPathPoint(PathPoint* pathPoint)
{
	mNavigationMap.insert(pathPoint);
}

void World::removeAllPathPoints()
{
	auto i = mNavigationMap.begin(), iEnd = mNavigationMap.end();
	while (i != iEnd)
	{
		PathPoint* point = (*i);
		i++;

		mNavigationMap.erase(point);
		delete point;
	}
}

void World::cleanDestroyedActors()
{
	auto i = mAllActors.begin(), iEnd = mAllActors.end();
	while (i != iEnd)
	{
		IActor* currentActor = (*i);
		i++;

		if (currentActor->isWaitDestruction())
		{
			removeActor(currentActor);
			delete currentActor;
		}
	}
}

IActor* World::getActorById(std::string id)
{
	for (const auto &actor : mAllActors)
	{
		if (id == actor->getActorId())
		{
			return actor;
		}
	}

	return nullptr;
}

std::set<IActor*>& World::getAllActors()
{
	return mAllActors;
}

const std::set<IActor*>& World::getAllActors() const
{
	return mAllActors;
}

const std::set<PathPoint*>& World::getNavigationMap() const
{
	return mNavigationMap;
}
