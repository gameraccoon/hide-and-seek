#ifndef WORLD_H
#define WORLD_H

#include <set>

#include "../src/Globals.h"
#include "../src/IActor.h"

class World
{
public:
	World(void);
	~World(void);
	/** Add Actor to the World. */
	void Spawn(IActor* actor);
	/** Delete actor from the World. */
	void Delete(IActor* actor);
	/** Update all Actors in the World */
	void Update(float deltaTime);
	/** All actors in the World. */
	std::set<IActor*> AllActors;
};

#endif