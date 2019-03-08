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

	/** SUPER NOT OPTIMAL for now */
	template<typename T>
	std::vector<std::shared_ptr<T>> getComponents() const
	{
		std::vector<std::shared_ptr<T>> resultVector;
		auto& actors = getAllActors();
		for (auto& actor : actors)
		{
			auto components = actor->getComponents<T>();
			resultVector.insert(resultVector.end(), components.begin(), components.end());
		}
		return resultVector;
	}

	std::shared_ptr<class CameraComponent> getMainCamera() { return mMainCamera; }
	void setMainCamera(std::shared_ptr<class CameraComponent> newCamera) { mMainCamera = newCamera; }

	std::shared_ptr<class TransformComponent> getPlayerControlledTransform() { return mPlayerControlledTransform; }
	void setPlayerControlledTransform(std::shared_ptr<class TransformComponent> newTransform) { mPlayerControlledTransform = newTransform; }

private:
	/** Destroy actors that waits to be destroyed */
	void cleanDestroyedActors();
	/** All actors in the World. */
	std::vector<IActor::Ptr> mAllActors;
	/** All pathpoints of this world */
	std::vector<PathPoint::Ptr> mNavigationMap;

	std::shared_ptr<class CameraComponent> mMainCamera;

	std::shared_ptr<class TransformComponent> mPlayerControlledTransform;
};
