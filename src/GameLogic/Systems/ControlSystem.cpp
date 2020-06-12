#include "Base/precomp.h"

#include "GameLogic/Systems/ControlSystem.h"

#include <sdl/SDL_keycode.h>

#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"
#include "GameData/Components/ImguiComponent.generated.h"
#include "GameData/Components/CharacterStateComponent.generated.h"
#include "GameData/Components/WorldCachedDataComponent.generated.h"
#include "GameData/Components/DebugDrawComponent.generated.h"

#include "GameData/World.h"
#include "GameData/GameData.h"


ControlSystem::ControlSystem(WorldHolder& worldHolder, HAL::Engine& engine, HAL::KeyStatesMap& keyStates)
	: mWorldHolder(worldHolder)
	, mEngine(engine)
	, mKeyStates(keyStates)
{
}

void UpdateRenderStateOnPressed(HAL::KeyStatesMap& keys, int key, bool& value)
{
	if (keys.isJustPressed(key))
	{
		value = !value;
	}
}

void ControlSystem::update()
{
	World& world = mWorldHolder.getWorld();
	GameData& gameData = mWorldHolder.getGameData();

#ifdef IMGUI_ENABLED
	if (auto [imgui] = gameData.getGameComponents().getComponents<ImguiComponent>(); imgui)
	{
		UpdateRenderStateOnPressed(mKeyStates, SDLK_F1, imgui->getIsImguiVisibleRef());
		if (imgui->getIsImguiVisible())
		{
			// stop processing input if imgui is shown
			return;
		}
	}
#endif // IMGUI_ENABLED

	bool isRunPressed = mKeyStates.isPressed(SDLK_LSHIFT) || mKeyStates.isPressed(SDLK_RSHIFT);

	Vector2D movementDirection(0.0f, 0.0f);
	if (mKeyStates.isPressed(SDLK_LEFT) || mKeyStates.isPressed(SDLK_a))
	{
		movementDirection += LEFT_DIRECTION;
	}

	if (mKeyStates.isPressed(SDLK_RIGHT) || mKeyStates.isPressed(SDLK_d))
	{
		movementDirection += RIGHT_DIRECTION;
	}

	if (mKeyStates.isPressed(SDLK_UP) || mKeyStates.isPressed(SDLK_w))
	{
		movementDirection += UP_DIRECTION;
	}

	if (mKeyStates.isPressed(SDLK_DOWN) || mKeyStates.isPressed(SDLK_s))
	{
		movementDirection += DOWN_DIRECTION;
	}

	std::optional<std::pair<EntityView, CellPos>> controlledEntity = world.getTrackedSpatialEntity(STR_TO_ID("ControlledEntity"));

	if (controlledEntity.has_value())
	{
		if (auto [characterState] = controlledEntity->first.getComponents<CharacterStateComponent>(); characterState != nullptr)
		{
			characterState->getBlackboardRef().setValue<bool>(CharacterStateBlackboardKeys::TryingToMove, !movementDirection.isZeroLength());
			characterState->getBlackboardRef().setValue<bool>(CharacterStateBlackboardKeys::ReadyToRun, isRunPressed);
		}

		auto [transform, movement] = controlledEntity->first.getComponents<TransformComponent, MovementComponent>();
		movement->setMoveDirection(movementDirection);

		std::optional<std::pair<EntityView, CellPos>> mainCamera = world.getTrackedSpatialEntity(STR_TO_ID("CameraEntity"));

		if (mainCamera.has_value())
		{
			auto [cameraTransform] = mainCamera->first.getComponents<TransformComponent>();
			if (cameraTransform == nullptr)
			{
				return;
			}

			Vector2D screenSize = Vector2D(static_cast<float>(mEngine.getWidth()), static_cast<float>(mEngine.getHeight()));
			Vector2D screenHalfSize = screenSize * 0.5f;
			Vector2D mouseScreenPos(mEngine.getMouseX(), mEngine.getMouseY());

			Vector2D mouseHeroPos = -mouseScreenPos + screenHalfSize + cameraTransform->getLocation();

			movement->setSightDirection(transform->getLocation() - mouseHeroPos);
		}
	}

	if (auto [renderMode] = gameData.getGameComponents().getComponents<RenderModeComponent>(); renderMode)
	{
		UpdateRenderStateOnPressed(mKeyStates, SDLK_F2, renderMode->getIsDrawDebugCollisionsEnabledRef());
		UpdateRenderStateOnPressed(mKeyStates, SDLK_F3, renderMode->getIsDrawDebugAiDataEnabledRef());
		UpdateRenderStateOnPressed(mKeyStates, SDLK_F4, renderMode->getIsDrawLightsEnabledRef());
		UpdateRenderStateOnPressed(mKeyStates, SDLK_F5, renderMode->getIsDrawVisibleEntitiesEnabledRef());
		UpdateRenderStateOnPressed(mKeyStates, SDLK_F6, renderMode->getIsDrawDebugCharacterInfoEnabledRef());
		UpdateRenderStateOnPressed(mKeyStates, SDLK_F7, renderMode->getIsDrawDebugPrimitivesEnabledRef());
		UpdateRenderStateOnPressed(mKeyStates, SDLK_F8, renderMode->getIsDrawDebugCellInfoEnabledRef());
	}
}
