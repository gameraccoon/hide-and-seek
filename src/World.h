#ifndef WORLD_H
#define WORLD_H

#include <set>

#include "../src/Globals.h"
#include "../src/IActor.h"
#include "../src/PathPoint.h"
#include <string>

class World
{
public:
	typedef std::set<IActor*> ActorsSet;
	typedef std::set<PathPoint*> PathPointsSet;

	World(HGE *hge);
	~World(void);
	/** Add Actor to the World. */
	void spawnActor(IActor* actor);
	/** Delete actor from the World. */
	void removeActor(IActor* actor);
	/** Update all Actors in the World */
	void update(float deltatime);
	/** Get a pointer to the Hge subsystem */
	HGE* getHge();
	/** Add a new PathPoint to the navigation map*/
	void addPathPoint(PathPoint* pathPoint);
	/** All actors in the World. */
	ActorsSet allActors;
	/** All pathpoints of this world */
	PathPointsSet navigationMap;
private:
	/** Pointer to the Hge subsystem */
	HGE *hge;
	/** Free memory of path points */
	void removeAllPathPoints();
	/** Destroy actors that waits to be destroyed */
	void cleanDestroyedActors();
};

#endif