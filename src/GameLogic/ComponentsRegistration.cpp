#include "GameLogic/ComponentsRegistration.h"

#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/CameraComponent.generated.h"
#include "GameData/Components/LightComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"
#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/NavMeshComponent.generated.h"

namespace ComponentsRegistration
{
	void RegisterComponents(ComponentFactory &componentFactory)
	{
		componentFactory.registerComponent<TransformComponent>();
		componentFactory.registerComponent<RenderComponent>();
		componentFactory.registerComponent<CollisionComponent>();
		componentFactory.registerComponent<MovementComponent>();
		componentFactory.registerComponent<CameraComponent>();
		componentFactory.registerComponent<LightComponent>();
		componentFactory.registerComponent<RenderModeComponent>();
		componentFactory.registerComponent<NavMeshComponent>();
		componentFactory.registerComponent<AiControllerComponent>();
	}
}
