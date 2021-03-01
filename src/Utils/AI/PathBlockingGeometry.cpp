#include "Base/precomp.h"

#include "Utils/AI/PathBlockingGeometry.h"

#include "GameData/Core/Hull.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"

#include "Utils/Geometry/ShapeOperations.h"

namespace PathBlockingGeometry
{
	void CalculatePathBlockingGeometry(std::vector<std::vector<Vector2D>>& outGeometry, const TupleVector<CollisionComponent*, TransformComponent*>& components)
	{
		outGeometry.clear();

		std::vector<ShapeOperations::MergedGeometry> mergedGeometry;
		for (const auto [collision, transform] : components)
		{
			const Hull& hull = collision->getGeometry();
			if (hull.type == HullType::Angular)
			{
				mergedGeometry.emplace_back(hull.borders, transform->getLocation());
			}
		}

		for (ShapeOperations::MergedGeometry& geometry : mergedGeometry)
		{
			const size_t pointsSize = geometry.borders.size();

			outGeometry.emplace_back();
			std::vector<Vector2D>& polygon = outGeometry.back();
			polygon.resize(pointsSize);

			for (size_t i = 0; i < pointsSize; ++i)
			{
				// light blocking geometry have the opposite winding order
				const Vector2D point = geometry.borders[pointsSize - 1 - i].a;
				polygon[i] = point;
			}
		}
	}
} // namespace PathBlockingGeometry
