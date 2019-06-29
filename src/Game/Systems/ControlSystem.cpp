#include "Game/Systems/ControlSystem.h"

#include <sdl/SDL_keycode.h>

#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"

#include "GameData/World.h"

ControlSystem::ControlSystem(HAL::Engine* engine, HAL::KeyStatesMap* keyStates)
	: mEngine(engine)
	, mKeyStates(keyStates)
{
}

template<typename F1, typename F2>
void UpdateRenderStateOnPressed(HAL::KeyStatesMap* keys, World* world, int key, F1 getFPtr, F2 setFPtr)
{
	if (keys->isJustPressed(key))
	{
		auto [renderMode] = world->getWorldComponents().getComponents<RenderModeComponent>();
		if (renderMode)
		{
			(renderMode->*setFPtr)(!(renderMode->*getFPtr)());
		}
	}
}

void ControlSystem::update(World* world, float dt)
{
	float speed = 50.0f;
	if (mKeyStates->isPressed(SDLK_LSHIFT) || mKeyStates->isPressed(SDLK_RSHIFT))
	{
		speed *= 3.0f;
	}

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

	NullableEntity mainCamera = world->getMainCamera();
	if (!mainCamera.isValid())
	{
		return;
	}

	auto [cameraTransform] = world->getEntityManger().getEntityComponents<TransformComponent>(mainCamera.getEntity());
	if (cameraTransform == nullptr)
	{
		return;
	}

	Vector2D screenHalfSize = Vector2D(static_cast<float>(mEngine->getWidth()), static_cast<float>(mEngine->getHeight())) * 0.5f;
	Vector2D mouseScreenPos(mEngine->getMouseX(), mEngine->getMouseY());

	Vector2D drawShift = screenHalfSize - cameraTransform->getLocation();

	if (NullableEntity controlledEntity = world->getPlayerControlledEntity(); controlledEntity.isValid())
	{
		auto [transform, movement] = world->getEntityManger().getEntityComponents<TransformComponent, MovementComponent>(controlledEntity.getEntity());

		if (transform != nullptr && movement != nullptr)
		{
			Vector2D oldLocation = transform->getLocation();
			Vector2D move = speed * movementDirection;
			movement->setSpeed(move);
			Vector2D controlledEntityPosition = oldLocation + move * dt;
			transform->setLocation(controlledEntityPosition);

			Vector2D direction = mouseScreenPos - oldLocation - drawShift;
			transform->setRotation(direction.rotation());
		}
	}

	UpdateRenderStateOnPressed(mKeyStates, world, SDLK_F1, &RenderModeComponent::getIsDrawDebugFpsEnabled, &RenderModeComponent::setIsDrawDebugFpsEnabled);
	UpdateRenderStateOnPressed(mKeyStates, world, SDLK_F2, &RenderModeComponent::getIsDrawDebugCollisionsEnabled, &RenderModeComponent::setIsDrawDebugCollisionsEnabled);
	UpdateRenderStateOnPressed(mKeyStates, world, SDLK_F3, &RenderModeComponent::getIsDrawDebugNavmeshEnabled, &RenderModeComponent::setIsDrawDebugNavmeshEnabled);
	UpdateRenderStateOnPressed(mKeyStates, world, SDLK_F4, &RenderModeComponent::getIsDrawLightsEnabled, &RenderModeComponent::setIsDrawLightsEnabled);
	UpdateRenderStateOnPressed(mKeyStates, world, SDLK_F5, &RenderModeComponent::getIsDrawVisibleEntitiesEnabled, &RenderModeComponent::setIsDrawVisibleEntitiesEnabled);
}
