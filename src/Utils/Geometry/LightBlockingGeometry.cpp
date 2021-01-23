#include "Base/precomp.h"

#include "Utils/Geometry/LightBlockingGeometry.h"

#include <numeric>

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"

#include "GameData/Spatial/SpatialWorldData.h"

#include "Utils/Geometry/Collide.h"
#include "Utils/Geometry/ShapeOperations.h"

namespace LightBlockingGeometry
{
	static void updateAABBsX(BoundingBox& box, float x)
	{
		if (x < box.minX) { box.minX = x; }
		if (x > box.maxX) { box.maxX = x; }
	}

	static void updateAABBsY(BoundingBox& box, float y)
	{
		if (y < box.minY) { box.minY = y; }
		if (y > box.maxY) { box.maxY = y; }
	}

	static bool isPointInsideAABB(const BoundingBox& box, Vector2D a)
	{
		return box.minX < a.x && a.x < box.maxX
			&& box.minY < a.y && a.y < box.maxY;
	}

	struct MergedGeometry
	{
		std::vector<SimpleBorder> borders;
		BoundingBox aabb
		{
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::lowest(),
			std::numeric_limits<float>::lowest()
		};
	};

	using CellGeometry = std::vector<MergedGeometry>;

	static bool AreShapesIntersect(const MergedGeometry& firstGeometry, const MergedGeometry& secondGeometry)
	{
		if (!Collide::AreAABBsIntersect(firstGeometry.aabb, secondGeometry.aabb))
		{
			return false;
		}

		for (SimpleBorder firstBorder : firstGeometry.borders)
		{
			for (SimpleBorder secondBorder : secondGeometry.borders)
			{
				if (Collide::AreLinesIntersect(firstBorder.a, firstBorder.b, secondBorder.a, secondBorder.b))
				{
					return true;
				}
			}
		}

		return false;
	}

	static Vector2D GetLeftmostPoint(const SimpleBorder& border)
	{
		if (border.a.x < border.b.x)
		{
			return border.a;
		}
		else if (border.b.x > border.a.x)
		{
			return border.b;
		}
		// if a.x and b.x are equal, compare by y axis
		else if (border.a.y < border.b.y)
		{
			return border.a;
		}
		else
		{
			return border.b;
		}
	}

	static void MergeGeometry(std::vector<SimpleBorder>& inOutResult, CellGeometry& inOutCellGeometry, const BoundingBox& cellAABB)
	{
		if (inOutCellGeometry.empty())
		{
			return;
		}

		for (size_t i = 0; i < inOutCellGeometry.size() - 1; ++i)
		{
			MergedGeometry& firstGeometry = inOutCellGeometry[i];
			for (size_t j = i + 1; j < inOutCellGeometry.size(); ++j)
			{
				MergedGeometry& secondGeometry = inOutCellGeometry[j];
				if (AreShapesIntersect(firstGeometry, secondGeometry))
				{
					std::vector<SimpleBorder> newShape = ShapeOperations::GetUnion(firstGeometry.borders, secondGeometry.borders);

					// save the new geometry to the position of the first figure
					firstGeometry.borders = newShape;
					// remove the second figure
					inOutCellGeometry.erase(inOutCellGeometry.begin() + j);
					// retry all collision tests with the first figure
					--i;
					break;
				}
			}
		}

		for (MergedGeometry& geometry : inOutCellGeometry)
		{
			for (SimpleBorder& border : geometry.borders)
			{
				if (isPointInsideAABB(cellAABB, border.a) && isPointInsideAABB(cellAABB, border.b))
				{
					inOutResult.push_back(border);
				}
				else
				{
					Vector2D leftmostPoint = GetLeftmostPoint(border);
					// only add to the cell where our left-most point
					if (isPointInsideAABB(cellAABB, leftmostPoint))
					{
						inOutResult.push_back(border);
					}
				}
			}
		}
	}

	void CalculateLightGeometry(CalculatedGeometry& outGeometry, const CollisionGeometry& collisionGeometry)
	{
		std::unordered_map<CellPos, CellGeometry> cellGeometry;
		for (const auto [cell, collision, transform] : collisionGeometry)
		{
			CellGeometry& currentCellGeometry = cellGeometry[cell->getPos()];
			currentCellGeometry.emplace_back();
			MergedGeometry& newGeometry = currentCellGeometry.back();

			Vector2D location = transform->getLocation();

			const Hull& hull = collision->getGeometry();
			newGeometry.borders.reserve(hull.borders.size());
			for (const Border& border : hull.borders)
			{
				newGeometry.borders.emplace_back(border.getA() + location, border.getB() + location);
				SimpleBorder& newBorder = newGeometry.borders.back();
				updateAABBsX(newGeometry.aabb, newBorder.a.x);
				updateAABBsY(newGeometry.aabb, newBorder.a.y);
				updateAABBsX(newGeometry.aabb, newBorder.b.x);
				updateAABBsY(newGeometry.aabb, newBorder.b.y);
			}

			CellPos currentCellPos = cell->getPos();
			std::vector<CellPos> secondaryCells;

			BoundingBox cellAABB = SpatialWorldData::GetCellAABB(cell->getPos());

			bool intersectsMinX = newGeometry.aabb.minX < cellAABB.minX;
			bool intersectsMaxX = newGeometry.aabb.maxX > cellAABB.maxX;
			bool intersectsMinY = newGeometry.aabb.minY < cellAABB.minY;
			bool intersectsMaxY = newGeometry.aabb.maxY > cellAABB.maxY;

			if (intersectsMinX)
			{
				cellGeometry[CellPos(currentCellPos.x - 1, currentCellPos.y)].push_back(newGeometry);
			}
			if (intersectsMaxX)
			{
				cellGeometry[CellPos(currentCellPos.x + 1, currentCellPos.y)].push_back(newGeometry);
			}
			if (intersectsMinY)
			{
				cellGeometry[CellPos(currentCellPos.x, currentCellPos.y - 1)].push_back(newGeometry);
			}
			if (intersectsMaxY)
			{
				cellGeometry[CellPos(currentCellPos.x, currentCellPos.y + 1)].push_back(newGeometry);
			}
			if (intersectsMinX && intersectsMinY)
			{
				cellGeometry[CellPos(currentCellPos.x - 1, currentCellPos.y - 1)].push_back(newGeometry);
			}
			if (intersectsMinX && intersectsMaxY)
			{
				cellGeometry[CellPos(currentCellPos.x - 1, currentCellPos.y + 1)].push_back(newGeometry);
			}
			if (intersectsMaxX && intersectsMinY)
			{
				cellGeometry[CellPos(currentCellPos.x + 1, currentCellPos.y - 1)].push_back(newGeometry);
			}
			if (intersectsMaxX && intersectsMaxY)
			{
				cellGeometry[CellPos(currentCellPos.x + 1, currentCellPos.y + 1)].push_back(newGeometry);
			}
		}

		// collect intersections inside cells
		for (auto& [pos, oneCellGeometry] : cellGeometry)
		{
			BoundingBox cellAABB = SpatialWorldData::GetCellAABB(pos);
			MergeGeometry(outGeometry[pos], oneCellGeometry, cellAABB);
		}
	}
}
