#include "Base/precomp.h"

#include "GameLogic/Systems/WeaponSystem.h"

#include "GameData/World.h"
#include "GameData/GameData.h"

#include "GameData/Components/WeaponComponent.generated.h"
#include "GameData/Components/CharacterStateComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"


WeaponSystem::WeaponSystem(WorldHolder& worldHolder, const TimeData& timeData)
	: mWorldHolder(worldHolder)
	, mTime(timeData)
{
}

void WeaponSystem::update()
{
	World& world = mWorldHolder.getWorld();
	GameplayTimestamp currentTime = mTime.currentTimestamp;

	world.getSpatialData().getAllCellManagers().forEachComponentSet<WeaponComponent, CharacterStateComponent, TransformComponent>([currentTime](WeaponComponent* weaponComponent, CharacterStateComponent* characterState, TransformComponent* /*transform*/)
	{
		if (characterState->getState() == CharacterState::Shoot || characterState->getState() == CharacterState::WalkAndShoot)
		{
			if (currentTime > weaponComponent->getShotFinishTimestamp())
			{
				// do next shot here
				weaponComponent->setShotFinishTimestamp(currentTime.getIncreasedByFloatTime(weaponComponent->getShotPeriod()));
			}
		}
	});
}
