#include "GameLogic/Systems/CollisionSystem.h"

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/World.h"

#include "Utils/Geometry/Collide.h"


CollisionSystem::CollisionSystem(WorldHolder& worldHolder, const TimeData& timeData)
	: mWorldHolder(worldHolder)
	, mTime(timeData)
{
}

void CollisionSystem::update()
{
	struct SpatialComponents
	{
		WorldCell* cell;
		std::vector<std::tuple<CollisionComponent*, TransformComponent*>> components;
	};

	World& world = mWorldHolder.getWorld();
//	const GameplayTimestamp timestampNow = mTime.currentTimestamp;
	float dt = mTime.dt;

	auto& allCellsMap = world.getSpatialData().getAllCells();
	std::vector<SpatialComponents> components(allCellsMap.size());
	size_t i = 0;
	for (auto& pair : allCellsMap)
	{
		pair.second.getEntityManager().getComponents<CollisionComponent, TransformComponent>(components[i].components);
		components[i].cell = &pair.second;
		++i;
	}

	for (auto& pair : components)
	{
		for (auto [collision, transform] : pair.components)
		{
			Collide::UpdateBoundingBox(collision);
		}
	}

	world.getSpatialData().getAllCellManagers().forEachSpatialComponentSet<CollisionComponent, TransformComponent, MovementComponent>([&components, dt](WorldCell* cell, CollisionComponent* collisionComponent, TransformComponent* transformComponent, MovementComponent* movementComponent)
	{
		CellPos cellPos = cell->getPos();
		Vector2D resist = ZERO_VECTOR;
		for (auto& pair : components)
		{
			CellPos cellDiff = pair.cell->getPos() - cellPos;
			for (auto [collision, transform] : pair.components)
			{
				Vector2D cellPosDiff(cellDiff.x * SpatialWorldData::CellSize, cellDiff.y * SpatialWorldData::CellSize);
				if (collision != collisionComponent)
				{
					bool doCollide = Collide::DoCollide(collisionComponent, transformComponent->getLocation() - cellPosDiff + movementComponent->getNextStep(), collision, transform->getLocation(), resist);

					if (doCollide)
					{
						movementComponent->setNextStep(movementComponent->getNextStep() + resist);
					}
				}
			}
		}
	});
}
