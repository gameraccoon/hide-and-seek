#include "World.h"

World::World(void)
{
}

World::~World(void)
{
	auto i = this->allActors.begin(), iEnd = this->allActors.end();

	while (i != iEnd)
	{
		IActor* actorToDelete = (*i);
		i++;
		
		this->allActors.erase(actorToDelete);

		delete (actorToDelete);
	}

	this->allActors.clear();
	this->removeAllPathPoints();
}

void World::spawnActor(IActor* actor)
{
	this->allActors.insert(actor);
}

void World::removeActor(IActor* actor)
{
	this->allActors.erase(actor);
}

void World::update(float deltatime)
{
	for (auto actor : this->allActors)
	{
		if (!actor->isWaitDestruction())
		{
			actor->update(deltatime);
		}
	}

	this->cleanDestroyedActors();
}

void World::addPathPoint(PathPoint* pathPoint)
{
	this->navigationMap.insert(pathPoint);
}

void World::removeAllPathPoints()
{
	auto i = this->navigationMap.begin(), iEnd = this->navigationMap.end();
	while (i != iEnd)
	{
		PathPoint* point = (*i);
		i++;

		this->navigationMap.erase(point);
		delete point;
	}
}

void World::cleanDestroyedActors()
{
	auto i = this->allActors.begin(), iEnd = this->allActors.end();
	while (i != iEnd)
	{
		IActor* currentActor = (*i);
		i++;

		if (currentActor->isWaitDestruction())
		{
			this->removeActor(currentActor);
			delete currentActor;
		}
	}
}

IActor* World::getActorById(std::string id)
{
	for (const auto &actor : this->allActors)
	{
		if (id == actor->getActorId())
		{
			return actor;
		}
	}

	return nullptr;
}