#include "GameLogic/Systems/ControlSystem.h"

#include <sdl/SDL_keycode.h>

#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"
#include "GameData/Components/CharacterStateComponent.generated.h"

#include "GameData/World.h"
#include "GameData/GameData.h"


ControlSystem::ControlSystem(WorldHolder& worldHolder, HAL::Engine* engine, HAL::KeyStatesMap* keyStates)
	: mWorldHolder(worldHolder)
	, mEngine(engine)
	, mKeyStates(keyStates)
{
}

template<typename F1, typename F2>
void UpdateRenderStateOnPressed(HAL::KeyStatesMap* keys, RenderModeComponent* renderMode, int key, F1 getFPtr, F2 setFPtr)
{
	if (keys->isJustPressed(key))
	{
		(renderMode->*setFPtr)(!(renderMode->*getFPtr)());
	}
}

void ControlSystem::update()
{
	World* world = mWorldHolder.world;
	GameData* gameData = mWorldHolder.gameData;

	bool isRunPressed = mKeyStates->isPressed(SDLK_LSHIFT) || mKeyStates->isPressed(SDLK_RSHIFT);

	Vector2D movementDirection(0.0f, 0.0f);
	if (mKeyStates->isPressed(SDLK_LEFT) || mKeyStates->isPressed(SDLK_a))
	{
		movementDirection += LEFT_DIRECTION;
	}

	if (mKeyStates->isPressed(SDLK_RIGHT) || mKeyStates->isPressed(SDLK_d))
	{
		movementDirection += RIGHT_DIRECTION;
	}

	if (mKeyStates->isPressed(SDLK_UP) || mKeyStates->isPressed(SDLK_w))
	{
		movementDirection += UP_DIRECTION;
	}

	if (mKeyStates->isPressed(SDLK_DOWN) || mKeyStates->isPressed(SDLK_s))
	{
		movementDirection += DOWN_DIRECTION;
	}

	OptionalEntity controlledEntity = world->getPlayerControlledEntity();
	if (controlledEntity.isValid())
	{
		if (auto [characterState] = world->getEntityManager().getEntityComponents<CharacterStateComponent>(controlledEntity.getEntity()); characterState != nullptr)
		{
			characterState->getBlackboardRef().setValue<bool>(CharacterStateBlackboardKeys::TryingToMove, !movementDirection.isZeroLength());
			characterState->getBlackboardRef().setValue<bool>(CharacterStateBlackboardKeys::ReadyToRun, isRunPressed);
		}
	}

	OptionalEntity mainCamera = world->getMainCamera();
	if (mainCamera.isValid())
	{
		auto [cameraTransform] = world->getEntityManager().getEntityComponents<TransformComponent>(mainCamera.getEntity());
		if (cameraTransform == nullptr)
		{
			return;
		}

		Vector2D screenHalfSize = Vector2D(static_cast<float>(mEngine->getWidth()), static_cast<float>(mEngine->getHeight())) * 0.5f;
		Vector2D mouseScreenPos(mEngine->getMouseX(), mEngine->getMouseY());

		Vector2D drawShift = screenHalfSize - cameraTransform->getLocation();

		if (controlledEntity.isValid())
		{
			auto [transform, movement] = world->getEntityManager().getEntityComponents<TransformComponent, MovementComponent>(controlledEntity.getEntity());

			movement->setMoveDirection(movementDirection);
			movement->setSightDirection(mouseScreenPos - transform->getLocation() - drawShift);
		}
	}


	auto [renderMode] = gameData->getGameComponents().getComponents<RenderModeComponent>();
	if (renderMode)
	{
		UpdateRenderStateOnPressed(mKeyStates, renderMode, SDLK_F1, &RenderModeComponent::getIsDrawDebugFpsEnabled, &RenderModeComponent::setIsDrawDebugFpsEnabled);
		UpdateRenderStateOnPressed(mKeyStates, renderMode, SDLK_F2, &RenderModeComponent::getIsDrawDebugCollisionsEnabled, &RenderModeComponent::setIsDrawDebugCollisionsEnabled);
		UpdateRenderStateOnPressed(mKeyStates, renderMode, SDLK_F3, &RenderModeComponent::getIsDrawDebugAiDataEnabled, &RenderModeComponent::setIsDrawDebugAiDataEnabled);
		UpdateRenderStateOnPressed(mKeyStates, renderMode, SDLK_F4, &RenderModeComponent::getIsDrawLightsEnabled, &RenderModeComponent::setIsDrawLightsEnabled);
		UpdateRenderStateOnPressed(mKeyStates, renderMode, SDLK_F5, &RenderModeComponent::getIsDrawVisibleEntitiesEnabled, &RenderModeComponent::setIsDrawVisibleEntitiesEnabled);
		UpdateRenderStateOnPressed(mKeyStates, renderMode, SDLK_F6, &RenderModeComponent::getIsDrawDebugCharacterInfoEnabled, &RenderModeComponent::setIsDrawDebugCharacterInfoEnabled);
	}
}
