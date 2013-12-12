#ifndef WORLD_H
#define WORLD_H

#include <set>

#include "../src/Globals.h"
#include "../src/IActor.h"
#include "../src/PathPoint.h"

class World
{
public:
	World(HGE *hge);
	~World(void);
	/** Add Actor to the World. */
	void Spawn(IActor* actor);
	/** Delete actor from the World. */
	void Delete(IActor* actor);
	/** Update all Actors in the World */
	void Update(float deltaTime);
	HGE* GetHge();
	/** Add nwe PathPoint to the navigation map*/
	void AddPathPoint(PathPoint* newPoint);
	/** All actors in the World. */
	std::set<IActor*> AllActors;
	/** All pathpoints of this world */
	std::set<PathPoint*> NavigationMap;
private:
	/** Pointer to the Hge subsystem */
	HGE *Hge;
	/** Free memory of path points */
	void RemoveAllPathPoints();
};

#endif