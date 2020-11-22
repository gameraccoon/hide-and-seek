#include "worldsaveutils.h"

#include "GameData/World.h"

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/LightBlockingGeometryComponent.generated.h"

#include "Utils/World/GameDataLoader.h"
#include "Utils/Geometry/LightBlockingGeometry.h"

namespace Utils
{
	template<typename Component>
	void RemoveCellComponent(World& world)
	{
		TupleVector<Entity, Component*> components;
		for (std::pair<const CellPos, WorldCell>& cell : world.getSpatialData().getAllCells())
		{
			cell.second.getCellComponents().removeComponent(Component::GetTypeName());
		}
	}

	static void RefreshLightBlockingGeometry(World& world)
	{
		RemoveCellComponent<LightBlockingGeometryComponent>(world);

		TupleVector<WorldCell*, CollisionComponent*, TransformComponent*> components;
		world.getSpatialData().getAllCellManagers().getSpatialComponents<CollisionComponent, TransformComponent>(components);
		std::unordered_map<CellPos, std::vector<SimpleBorder>> lightBlockingGeometryPieces;
		LightBlockingGeometry::CalculateLightGeometry(lightBlockingGeometryPieces, components);

		for (auto& [cellPos, borders] : lightBlockingGeometryPieces)
		{
			WorldCell& cell = world.getSpatialData().getOrCreateCell(cellPos);
			ComponentSetHolder& cellComponents = cell.getCellComponents();
			LightBlockingGeometryComponent* lightBlockingGeometry = cellComponents.getOrAddComponent<LightBlockingGeometryComponent>();
			lightBlockingGeometry->setBorders(std::move(borders));
		}
	}

	void SaveWorld(World& world, const std::string& fileName, const ComponentSerializersHolder& serializationHolder)
	{
		RefreshLightBlockingGeometry(world);
		world.clearCaches();
		GameDataLoader::SaveWorld(world, fileName, serializationHolder);
	}
}
