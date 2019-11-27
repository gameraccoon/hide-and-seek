#pragma once

#include "ECS/EntityManager.h"
#include "ECS/ComponentSetHolder.h"
#include "ECS/EntityManagerGroup.h"

#include "GameData/Spatial/WorldCell.h"
#include "GameData/Spatial/CellPos.h"
#include "GameData/Core/Vector2D.h"

class SpatialWorldData
{
public:
	/**
	 * @brief getCellsAround returns cells that are inside or toucing a rect that have center in
	 * `centerPosition` and size of `rect`
	 * @param baseCell cell that we calculate position from
	 * @param centerPosition center of the rectangle
	 * @param rect sizes of the rectangle
	 * @return pointers to loaded cells meeting the criteria
	 */
	std::vector<WorldCell*> getCellsAround(CellPos baseCell, const Vector2D& centerPosition, const Vector2D rect);
	EntityManagerGroup getCellManagersAround(CellPos baseCell, const Vector2D& centerPosition, const Vector2D rect);
	WorldCell* getCell(const CellPos& pos);
	WorldCell& getOrCreateCell(const CellPos& pos);

	std::unordered_map<CellPos, WorldCell>& getCells() { return mCells; }

	static CellPos CellPosFromVector2D(const Vector2D& pos);

	nlohmann::json toJson(const ComponentFactory& componentFactory) const;
	void fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory);

private:
	std::unordered_map<CellPos, WorldCell> mCells;
};
