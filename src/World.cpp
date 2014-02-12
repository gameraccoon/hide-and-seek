#include "World.h"

World::World(HGE* hge)
{
	Hge = hge;
	std::set<IActor*> AllActors;
}

World::~World(void)
{
	std::set<IActor*>::iterator it, next = AllActors.begin(), end = AllActors.end();

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

void World::Update(float deltaTime)
{
	for (std::set<IActor*>::iterator it = AllActors.begin(); it != AllActors.end(); it++)
	{
		(*it)->Update(deltaTime);
	}
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
	for (std::set<PathPoint*>::iterator it = NavigationMap.begin(), end = NavigationMap.end(); it != end; it++)
	{
		delete (*it);
	}
}
