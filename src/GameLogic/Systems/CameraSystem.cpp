#include "GameLogic/Systems/CameraSystem.h"

#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/WorldCachedDataComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/World.h"
#include "GameData/GameData.h"


CameraSystem::CameraSystem(WorldHolder& worldHolder, HAL::Engine& engine)
	: mWorldHolder(worldHolder)
	, mEngine(engine)
{
}

void CameraSystem::update()
{
	World& world = mWorldHolder.getWorld();

	std::optional<std::pair<EntityView, CellPos>> controlledEntity = world.getTrackedSpatialEntity(STR_TO_ID("ControlledEntity"));

	if (controlledEntity.has_value())
	{
		std::optional<std::pair<EntityView, CellPos>> mainCamera = world.getTrackedSpatialEntity(STR_TO_ID("CameraEntity"));

		if (mainCamera.has_value())
		{
			auto [cameraTransform, cameraMovement] = mainCamera->first.getComponents<TransformComponent, MovementComponent>();
			if (cameraTransform == nullptr)
			{
				return;
			}

			Vector2D screenSize = Vector2D(static_cast<float>(mEngine.getWidth()), static_cast<float>(mEngine.getHeight()));
			Vector2D screenHalfSize = screenSize * 0.5f;
			Vector2D mouseScreenPos(mEngine.getMouseX(), mEngine.getMouseY());

			const float cameraMobilityRate = 0.7f;

			auto [controledEntityTransform] = controlledEntity->first.getComponents<TransformComponent>();

			Vector2D cameraCellShift = SpatialWorldData::GetCellRealDistance(controlledEntity->second - mainCamera->second);

			Vector2D cameraNewPos = controledEntityTransform->getLocation() + cameraCellShift + (mouseScreenPos - screenHalfSize) * cameraMobilityRate;
			Vector2D cameraMove = cameraNewPos - cameraTransform->getLocation();

			cameraMovement->setNextStep(cameraMove);

			auto [worldCachedData] = world.getWorldComponents().getComponents<WorldCachedDataComponent>();
			if (worldCachedData == nullptr)
			{
				world.getWorldComponents().addComponent<WorldCachedDataComponent>();
				std::tie(worldCachedData) = world.getWorldComponents().getComponents<WorldCachedDataComponent>();
			}
			worldCachedData->setCameraPos(cameraNewPos);
			worldCachedData->setCameraCellPos(mainCamera->second);
			worldCachedData->setScreenSize(screenSize);
		}
	}
}
