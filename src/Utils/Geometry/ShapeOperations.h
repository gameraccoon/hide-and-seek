#pragma once

#include <vector>
#include <limits>

#include "GameData/Core/Vector2D.h"
#include "GameData/Core/Border.h"
#include "GameData/Core/BoundingBox.h"

namespace ShapeOperations
{
	std::vector<SimpleBorder> GetUnion(const std::vector<SimpleBorder>& shape1, const std::vector<SimpleBorder>& shape2);
	// join all borders that make a straight line
	void OptimizeShape(std::vector<SimpleBorder>& inOutShape);

	struct MergedGeometry
	{
		MergedGeometry(const std::vector<Border>& inBorders, Vector2D location);
		MergedGeometry(const std::vector<SimpleBorder>& simpleBorders);

		std::vector<SimpleBorder> borders;
		BoundingBox aabb
		{
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::lowest(),
			std::numeric_limits<float>::lowest()
		};
	};

	bool AreShapesIntersect(const MergedGeometry& firstGeometry, const MergedGeometry& secondGeometry);

	void MergeGeometry(std::vector<MergedGeometry>& inOutCellGeometry);
}
