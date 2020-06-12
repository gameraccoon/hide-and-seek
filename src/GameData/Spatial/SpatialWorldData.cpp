#include "Base/precomp.h"

#include "GameData/World.h"

#include <cmath>

#include <nlohmann/json.hpp>

#include "GameData/Components/TransformComponent.generated.h"

std::vector<WorldCell*> SpatialWorldData::getCellsAround(const CellPos& baseCell, const Vector2D& centerPosition, const Vector2D& rect)
{
	CellPos ltCell = CellPos(
		baseCell.x + static_cast<int>((centerPosition.x - rect.x * 0.5f - MaxObjectSize) / CellSize - 1),
		baseCell.y + static_cast<int>((centerPosition.y - rect.y * 0.5f - MaxObjectSize) / CellSize - 1)
	);

	CellPos rbCell = CellPos(
		baseCell.x + static_cast<int>((centerPosition.x + rect.x * 0.5f + MaxObjectSize) / CellSize),
		baseCell.y + static_cast<int>((centerPosition.y + rect.y * 0.5f + MaxObjectSize) / CellSize)
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

std::pair<CellPos, Vector2D> SpatialWorldData::TransformCellFromOldSize(const Vector2D& pos, const CellPos& oldPos, const Vector2D& oldSize)
{
	auto result = std::make_pair(CellPos(0, 0), pos + Vector2D(oldPos.x * oldSize.x, oldPos.y * oldSize.y));
	TransformCellPos(result.first, result.second);
	return result;
}

Vector2D SpatialWorldData::GetCellRealDistance(const CellPosDiff& cellDiff)
{
	return Vector2D(cellDiff.x * CellSize, cellDiff.y * CellSize);
}

nlohmann::json SpatialWorldData::toJson(const ComponentFactory& componentFactory) const
{
	nlohmann::json cellsJson;
	for (auto& cell : mCells)
	{
		cellsJson.push_back({
			{"pos", cell.first},
			{"cell", cell.second.toJson(componentFactory)}
		});
	}

	return nlohmann::json{
		{"cells", cellsJson},
		{"cell_size", CellSizeInt}
	};
}

static void RedistributeSpatialEntitiesBetweenCells(SpatialWorldData& spatialData, float oldCellSize)
{
	struct CellEntities
	{
		TupleVector<Entity, TransformComponent*> entities;
		WorldCell& cell;

		explicit CellEntities(WorldCell& cell) : cell(cell) {}
	};

	std::vector<CellEntities> entitiesToMove;

	// collect snapshot of entity locations
	for (auto& cellPair : spatialData.getAllCells())
	{
		CellEntities& newGroup = entitiesToMove.emplace_back(cellPair.second);
		cellPair.second.getEntityManager().getComponentsWithEntities<TransformComponent>(newGroup.entities);
	}

	Vector2D oldSize(oldCellSize, oldCellSize);

	// move entities to new cells
	for (auto& cellData : entitiesToMove)
	{
		for (auto [entity, transform] : cellData.entities)
		{
			std::pair<CellPos, Vector2D> newPos = SpatialWorldData::TransformCellFromOldSize(transform->getLocation(), cellData.cell.getPos(), oldSize);
			transform->setLocation(newPos.second);
			if (cellData.cell.getPos() != newPos.first)
			{
				WorldCell& newCell = spatialData.getOrCreateCell(newPos.first);
				cellData.cell.getEntityManager().transferEntityTo(newCell.getEntityManager(), entity);
			}
		}
	}
}

void SpatialWorldData::fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	const auto& cellsJson = json.at("cells");
	for (const auto& cellJson : cellsJson)
	{
		CellPos pos = cellJson.at("pos");
		auto res = mCells.emplace(pos, pos);
		res.first->second.fromJson(cellJson.at("cell"), componentFactory);
	}

	int cellSize = json.at("cell_size");
	if (cellSize != CellSizeInt)
	{
		RedistributeSpatialEntitiesBetweenCells(*this, static_cast<float>(cellSize));
	}

	initAbsoluteLocations();
}

void SpatialWorldData::initAbsoluteLocations()
{
	getAllCellManagers().forEachSpatialComponentSet<TransformComponent>([](WorldCell* cell, TransformComponent* transform)
	{
		CellPos cellPos = cell->getPos();
		Vector2D relativeLocation = transform->getLocation();
		transform->setLocation(Vector2D(cellPos.x * CellSize + relativeLocation.x, cellPos.y * CellSize + relativeLocation.y));
	});
}
