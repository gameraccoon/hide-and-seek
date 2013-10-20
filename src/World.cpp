#include "World.h"

World::World(void)
{
	std::set<IActor*> AllActors;
}

World::~World(void)
{
	AllActors.clear();
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
