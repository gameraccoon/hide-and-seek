#ifndef WORLD_H
#define WORLD_H

#include <set>

#include "../src/Globals.h"
#include "../src/Actor.h"

class World
{
public:
	World(void);
	~World(void);
	/** Add Actor to the World. */
	void Spawn(Actor* actor);
	/** Delete actor form the World. */
	void Delete(Actor* actor);
	/** TEST METHOD - Render all Actors in the World. */
	void RenderAll();
	/** Update all Actors in the World */
	void Update(float deltaTime);

private:
	/** All actors in the World. */
	std::set<Actor*> AllActors;
};

#endif