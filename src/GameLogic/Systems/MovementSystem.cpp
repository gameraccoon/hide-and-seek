#include "GameLogic/Systems/MovementSystem.h"

#include <sdl/SDL_keycode.h>

#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"

#include "GameData/World.h"


MovementSystem::MovementSystem(WorldHolder& worldHolder, const TimeData& timeData)
	: mWorldHolder(worldHolder)
	, mTime(timeData)
{
}

void MovementSystem::update()
{
	World& world = mWorldHolder.getWorld();
	const float dt = mTime.dt;
	const GameplayTimestamp timestampNow = mTime.currentTimestamp;

	world.getEntityManager().forEachComponentSet<MovementComponent, TransformComponent>([dt, timestampNow](MovementComponent* movement, TransformComponent* transform) {
		float speed = movement->getSpeed();
		if (speed > 0.0f)
		{
			transform->setLocation(transform->getLocation() + movement->getMoveDirection().unit() * speed * dt);
			transform->setRotation(movement->getSightDirection().rotation());
			transform->setUpdateTimestamp(timestampNow);
		}
	});
}
