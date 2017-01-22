#pragma once

#include <set>

#include "IActor.h"
#include "../Structures/PathPoint.h"

#include <string>

class World
{
public:
	World();
	~World();
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
	/** */
	std::set<IActor*>& getAllActors();
	/** */
	const std::set<IActor*>& getAllActors() const;
	/** */
	const std::set<PathPoint*>& getNavigationMap() const;
private:
	/** Free memory of path points */
	void removeAllPathPoints();
	/** Destroy actors that waits to be destroyed */
	void cleanDestroyedActors();
	/** All actors in the World. */
	std::set<IActor*> mAllActors;
	/** All pathpoints of this world */
	std::set<PathPoint*> mNavigationMap;
};
