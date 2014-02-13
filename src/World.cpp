#include "World.h"

World::World(HGE* hge)
{
	this->hge = hge;
}

World::~World(void)
{
	ActorsSet::iterator i, iNext = this->allActors.begin(), iEnd = this->allActors.end();

	while (iNext != iEnd)
	{
		i = iNext;
		iNext++;
		
		this->allActors.erase(i);

		delete (*i);
	}

	delete (*iEnd);

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
	for (ActorsSet::iterator i = this->allActors.begin(), iEnd = this->allActors.end(); i != iEnd; i++)
	{
		(*i)->update(deltatime);
	}

	this->cleanDestroyedActors();
}

HGE* World::getHge()
{
	return this->hge;
}

void World::addPathPoint(PathPoint* pathPoint)
{
	this->navigationMap.insert(pathPoint);
}

void World::removeAllPathPoints()
{
	for (PathPointsSet::iterator i = this->navigationMap.begin(), iEnd = this->navigationMap.end(); i != iEnd; i++)
	{
		delete (*i);
	}
}

void World::cleanDestroyedActors()
{
	for (ActorsSet::iterator i = this->allActors.begin(), iEnd = this->allActors.end(); i != iEnd; i++)
	{
		if ((*i)->isWaitDestruction())
		{
			IActor* actorToDelete = (*i);
			this->allActors.erase(actorToDelete);
			delete actorToDelete;
		}
	}
}