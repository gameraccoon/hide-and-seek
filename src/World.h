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
	void Spawn(IActor* actor);
	/** Delete actor from the World. */
	void Delete(IActor* actor);
	/** Update all Actors in the World */
	void Update(float deltatime);
	/** Get a pointer to the Hge subsystem */
	HGE* GetHge();
	/** Add a new PathPoint to the navigation map*/
	void AddPathPoint(PathPoint* newPoint);
	/** All actors in the World. */
	ActorsSet AllActors;
	/** All pathpoints of this world */
	PathPointsSet NavigationMap;
private:
	/** Pointer to the Hge subsystem */
	HGE *Hge;
	/** Free memory of path points */
	void RemoveAllPathPoints();
	/** Destroy actors that waits to be destroyed */
	void CleanDestroyedActors();
};

#endif