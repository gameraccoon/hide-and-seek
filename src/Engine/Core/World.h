#ifndef WORLD_H
#define WORLD_H

#include <set>

#include "IActor.h"
#include "../Structures/PathPoint.h"
#include "../Lua/LuaInstance.h"

#include <string>

class World
{
public:
	World(void);
	~World(void);
	/** Add Actor to the World. */
	void spawnActor(IActor* actor);
	/** Delete actor from the World. */
	void removeActor(IActor* actor);
	/** Update all Actors in the World */
	void update(float deltatime);
	/** Add a new PathPoint to the navigation map*/
	void addPathPoint(PathPoint* pathPoint);
	/** */
	IActor* getActorById(std::string id);
	/** All actors in the World. */
	std::set<IActor*> allActors;
	/** All pathpoints of this world */
	std::set<PathPoint*> navigationMap;
private:
	/** Free memory of path points */
	void removeAllPathPoints();
	/** Destroy actors that waits to be destroyed */
	void cleanDestroyedActors();
};

#endif