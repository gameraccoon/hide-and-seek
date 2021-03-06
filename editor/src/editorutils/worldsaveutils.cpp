#include "worldsaveutils.h"

#include "GameData/World.h"

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/LightBlockingGeometryComponent.generated.h"
#include "GameData/Components/PathBlockingGeometryComponent.generated.h"

#include "Utils/World/GameDataLoader.h"
#include "Utils/Geometry/LightBlockingGeometry.h"
#include "Utils/AI/PathBlockingGeometry.h"

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
			// try to stabilize borders between saves
			std::sort(
				borders.begin(),
				borders.end(),
				[](const SimpleBorder& first, const SimpleBorder& second){
					return first.a.x < second.a.x
						|| (first.a.x == second.a.x &&
							(first.b.x < second.b.x
							|| (first.b.x == second.b.x &&
								(first.a.y < second.a.y
								|| (first.a.y == second.a.y &&
									first.b.y < second.b.y)))));
				}
			);

			WorldCell& cell = world.getSpatialData().getOrCreateCell(cellPos);
			ComponentSetHolder& cellComponents = cell.getCellComponents();
			LightBlockingGeometryComponent* lightBlockingGeometry = cellComponents.getOrAddComponent<LightBlockingGeometryComponent>();
			lightBlockingGeometry->setBorders(std::move(borders));
		}
	}

	static void RefreshPathBlockingGeometry(World& world)
	{
		TupleVector<CollisionComponent*, TransformComponent*> components;
		world.getSpatialData().getAllCellManagers().getComponents<CollisionComponent, TransformComponent>(components);

		PathBlockingGeometryComponent* pathBlockingGeometry = world.getWorldComponents().getOrAddComponent<PathBlockingGeometryComponent>();

		PathBlockingGeometry::CalculatePathBlockingGeometry(pathBlockingGeometry->getPolygonsRef(), components);
	}

	void SaveWorld(World& world, const std::string& fileName, const ComponentSerializersHolder& serializationHolder)
	{
		RefreshLightBlockingGeometry(world);
		RefreshPathBlockingGeometry(world);
		world.packForJsonSaving();
		world.clearCaches();
		GameDataLoader::SaveWorld(world, fileName, serializationHolder);
	}
}
