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
	typedef std::set<IActor*> ActorsSet;
	typedef std::set<PathPoint*> PathPointsSet;

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
	/** All actors in the World. */
	ActorsSet allActors;
	/** All pathpoints of this world */
	PathPointsSet navigationMap;
private:
	/** Free memory of path points */
	void removeAllPathPoints();
	/** Destroy actors that waits to be destroyed */
	void cleanDestroyedActors();
};

#endif