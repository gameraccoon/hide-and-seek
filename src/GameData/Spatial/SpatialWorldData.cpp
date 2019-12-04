#include "GameData/World.h"

#include <cmath>

#include <nlohmann/json.hpp>

std::vector<WorldCell*> SpatialWorldData::getCellsAround(CellPos baseCell, const Vector2D& centerPosition, const Vector2D rect)
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

EntityManagerGroup SpatialWorldData::getCellManagersAround(CellPos baseCell, const Vector2D& centerPosition, const Vector2D rect)
{
	std::vector<WorldCell*> cells = getCellsAround(baseCell, centerPosition, rect);
	std::vector<EntityManager*> entityManagers;
	entityManagers.reserve(cells.size());
	for (WorldCell* cell : cells)
	{
		entityManagers.push_back(&cell->getEntityManager());
	}
	return EntityManagerGroup(entityManagers);
}

WorldCell* SpatialWorldData::getCell(const CellPos& pos)
{
	auto it = mCells.find(pos);
	return (it != mCells.end()) ? &it->second : nullptr;
}

WorldCell& SpatialWorldData::getOrCreateCell(const CellPos& pos)
{
	return mCells[pos];
}

EntityManagerGroup SpatialWorldData::getAllCellManagers()
{
	std::vector<EntityManager*> entityManagers;
	entityManagers.reserve(mCells.size());
	for (std::pair<const CellPos, WorldCell>& cell : mCells)
	{
		entityManagers.push_back(&cell.second.getEntityManager());
	}
	return EntityManagerGroup(entityManagers);
}

bool SpatialWorldData::TransformCellPos(CellPos& cellPos, Vector2D& pos)
{
	if (pos.isInside(ZERO_VECTOR, CellSizeVector))
	{
		return false;
	}

	int posXDiff = static_cast<int>(std::floor(pos.x / CellSize));
	int posYDiff = static_cast<int>(std::floor(pos.y / CellSize));

	pos.x -= posXDiff * CellSize;
	pos.y -= posYDiff * CellSize;

	cellPos.x += posXDiff;
	cellPos.y += posYDiff;

	return true;
}

std::pair<CellPos, Vector2D> SpatialWorldData::GetTransformedCellPos(CellPos oldCellPos, Vector2D oldPos)
{
	auto result = std::make_pair(oldCellPos, oldPos);
	TransformCellPos(result.first, result.second);
	return result;
}

CellPos SpatialWorldData::CellPosFromVector2D(const Vector2D& pos)
{
	return CellPos(static_cast<int>(pos.x / CellSize), static_cast<int>(pos.y / CellSize));
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
