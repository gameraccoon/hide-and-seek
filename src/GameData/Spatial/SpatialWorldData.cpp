#include "GameData/World.h"

#include <nlohmann/json.hpp>

std::vector<WorldCell*> SpatialWorldData::getCellsAround(CellPos baseCell, const Vector2D& centerPosition, const Vector2D rect)
{
	std::vector<WorldCell*> result;
	size_t maxCellRadius = static_cast<size_t>(std::max(rect.x, rect.y) / WorldCell::CellSize);
	result.reserve((1+maxCellRadius*2) * (1+maxCellRadius*2));

	CellPos ltCell = CellPos(
		static_cast<int>(baseCell.x + (centerPosition.x - rect.x * 0.5f) / WorldCell::CellSize),
		static_cast<int>(baseCell.y + (centerPosition.y - rect.y * 0.5f) / WorldCell::CellSize));

	CellPos rbCell = CellPos(
		static_cast<int>(baseCell.x + (centerPosition.x + rect.x * 0.5f) / WorldCell::CellSize),
		static_cast<int>(baseCell.y + (centerPosition.y + rect.y * 0.5f) / WorldCell::CellSize));

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

CellPos SpatialWorldData::CellPosFromVector2D(const Vector2D& pos)
{
	return CellPos(static_cast<int>(pos.x / WorldCell::CellSize), static_cast<int>(pos.y / WorldCell::CellSize));
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
