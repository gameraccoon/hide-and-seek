#pragma once

#include "IActor.h"
#include "Structures/PathPoint.h"

#include <string>

class World
{
public:
	World() = default;
	~World();
	/** Add Actor to the World. */
	void addActor(IActor::Ptr& actor);
	/** Update all Actors in the World */
	void update(float deltatime);
	/** Add a new PathPoint to the navigation map*/
	void addPathPoint(PathPoint::Ptr& pathPoint);
	/** */
	IActor* getActorById(const std::string& id);
	/** */
	std::vector<IActor::Ptr>& getAllActors();
	/** */
	const std::vector<IActor::Ptr>& getAllActors() const;
	/** */
	const std::vector<PathPoint::Ptr>& getNavigationMap() const;

private:
	/** Destroy actors that waits to be destroyed */
	void cleanDestroyedActors();
	/** All actors in the World. */
	std::vector<IActor::Ptr> mAllActors;
	/** All pathpoints of this world */
	std::vector<PathPoint::Ptr> mNavigationMap;
};
