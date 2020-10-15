#include "Base/precomp.h"

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"

#include "Utils/Geometry/LightBlockingGeometry.h"

namespace LightBlockingGeometry
{
	void CalculateLightGeometry(std::vector<CalculatedGeometry>& outGeometry, const CollisionGeometry& collisionGeometry)
	{
		// mock-implimentation, to be replaced with actual merging algorithm
		outGeometry.reserve(collisionGeometry.size());
		for (auto collisionGeometryTuple : collisionGeometry)
		{
			CollisionComponent* collision = std::get<2>(collisionGeometryTuple);
			TransformComponent* transform = std::get<3>(collisionGeometryTuple);
			outGeometry.emplace_back(collision->getGeometry(), transform->getLocation());
		}
	}
}
