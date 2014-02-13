#include "World.h"

World::World(HGE* hge)
{
	Hge = hge;
}

World::~World(void)
{
	ActorsSet::iterator it, next = AllActors.begin(), end = AllActors.end();

	while (next != end)
	{
		it = next;
		next++;
		
		AllActors.erase(it);

		delete (*it);
	}

	delete (*end);

	AllActors.clear();
	RemoveAllPathPoints();
}

void World::Spawn(IActor* actor)
{
	AllActors.insert(actor);
}

void World::Delete(IActor* actor)
{
	AllActors.erase(actor);
}

void World::Update(float deltatime)
{
	for (ActorsSet::iterator it = AllActors.begin(), end = AllActors.end(); it != end; it++)
	{
		(*it)->Update(deltatime);
	}

	CleanDestroyedActors();
}

HGE* World::GetHge()
{
	return Hge;
}

void World::AddPathPoint(PathPoint* newPoint)
{
	NavigationMap.insert(newPoint);
}

void World::RemoveAllPathPoints()
{
	for (PathPointsSet::iterator it = NavigationMap.begin(), end = NavigationMap.end(); it != end; it++)
	{
		delete (*it);
	}
}

void World::CleanDestroyedActors()
{
	for (ActorsSet::iterator it = AllActors.begin(), end = AllActors.end(); it != end; it++)
	{
		if ((*it)->IsWaitDestruction())
		{
			IActor* actorToDelete = (*it);
			AllActors.erase(actorToDelete);
			delete actorToDelete;
		}
	}
}