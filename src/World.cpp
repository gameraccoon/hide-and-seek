#include "World.h"

World::World(void)
{
	std::set<Actor*> AllActors;
}

World::~World(void)
{
	AllActors.clear();
}

void World::Spawn(Actor* actor)
{
	AllActors.insert(actor);
}

void World::Delete(Actor* actor)
{
	AllActors.erase(actor);
}

void World::RenderAll()
{
	for (std::set<Actor*>::iterator it = AllActors.begin(); it != AllActors.end(); it++)
	{
		(*it)->Render();
	}
}

void World::Update(float deltaTime)
{
	for (std::set<Actor*>::iterator it = AllActors.begin(); it != AllActors.end(); it++)
	{
		(*it)->Update(deltaTime);
	}
}
