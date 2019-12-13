#include "GameData/World.h"

#include <cmath>

#include <nlohmann/json.hpp>

std::vector<WorldCell*> SpatialWorldData::getCellsAround(const CellPos& baseCell, const Vector2D& centerPosition, const Vector2D& rect)
{
	CellPos ltCell = CellPos(
		static_cast<int>(baseCell.x + (centerPosition.x - rect.x * 0.5f - MaxObjectSize) / CellSize),
		static_cast<int>(baseCell.y + (centerPosition.y - rect.y * 0.5f - MaxObjectSize) / CellSize)
	);

	CellPos rbCell = CellPos(
		static_cast<int>(baseCell.x + (centerPosition.x + rect.x * 0.5f + MaxObjectSize) / CellSize),
		static_cast<int>(baseCell.y + (centerPosition.y + rect.y * 0.5f + MaxObjectSize) / CellSize)
	);

	std::vector<WorldCell*> result;
	result.reserve(static_cast<size_t>((rbCell.x - ltCell.x) * (rbCell.y - ltCell.y)));

	for (int i = ltCell.x; i <= rbCell.x; ++i)
	{
		for (int j = ltCell.y; j <= rbCell.y; ++j)
		{
			auto it = mCells.find(CellPos(i, j));
			if (it != mCells.end())
			{
				result.push_back(&it->second);
			}
		}
	}

	return result;
}

SpatialEntityManager SpatialWorldData::getCellManagersAround(const CellPos& baseCell, const Vector2D& centerPosition, const Vector2D& rect)
{
	return SpatialEntityManager(getCellsAround(baseCell, centerPosition, rect));
}

WorldCell* SpatialWorldData::getCell(const CellPos& pos)
{
	auto it = mCells.find(pos);
	return (it != mCells.end()) ? &it->second : nullptr;
}

WorldCell& SpatialWorldData::getOrCreateCell(const CellPos& pos)
{
	if (auto it = mCells.find(pos); it != mCells.end())
	{
		return it->second;
	}

	auto pair = mCells.emplace(pos, pos);
	return pair.first->second;
}

SpatialEntityManager SpatialWorldData::getAllCellManagers()
{
	std::vector<WorldCell*> cells;
	cells.reserve(mCells.size());
	for (std::pair<const CellPos, WorldCell>& cell : mCells)
	{
		cells.push_back(&cell.second);
	}
	return SpatialEntityManager(cells);
}

Vector2D SpatialWorldData::GetRelativeLocation(const CellPos& baseCell, const CellPos& targetCell, const Vector2D& targetPos)
{
	return GetCellRealDistance(targetCell - baseCell) + targetPos;
}

bool SpatialWorldData::TransformCellPos(CellPos& inOutCellPos, Vector2D& inOutPos)
{
	if (inOutPos.isInside(ZERO_VECTOR, CellSizeVector))
	{
		return false;
	}

	int posXDiff = static_cast<int>(std::floor(inOutPos.x / CellSize));
	int posYDiff = static_cast<int>(std::floor(inOutPos.y / CellSize));

	inOutPos.x -= posXDiff * CellSize;
	inOutPos.y -= posYDiff * CellSize;

	inOutCellPos.x += posXDiff;
	inOutCellPos.y += posYDiff;

	return true;
}

std::pair<CellPos, Vector2D> SpatialWorldData::GetTransformedCellPos(const CellPos& oldCellPos, const Vector2D& oldPos)
{
	auto result = std::make_pair(oldCellPos, oldPos);
	TransformCellPos(result.first, result.second);
	return result;
}

CellPos SpatialWorldData::CellPosFromVector2D(const Vector2D& pos)
{
	return CellPos(static_cast<int>(pos.x / CellSize), static_cast<int>(pos.y / CellSize));
}

Vector2D SpatialWorldData::GetCellRealDistance(const CellPosDiff& cellDiff)
{
	return Vector2D(cellDiff.x * CellSize, cellDiff.y * CellSize);
}

nlohmann::json SpatialWorldData::toJson(const ComponentFactory& /*componentFactory*/) const
{
	return nlohmann::json{
//		{"entity_manager", mEntityManager.toJson(componentFactory)},
	};
}

void SpatialWorldData::fromJson(const nlohmann::json& /*json*/, const ComponentFactory& /*componentFactory*/)
{
//	mEntityManager.fromJson(json.at("entity_manager"), componentFactory);
}
