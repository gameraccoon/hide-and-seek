#include "componentregistration.h"

#include "ECS/ComponentFactory.h"

#include "generated/AiControllerComponentEditFactory.generated.h"
#include "generated/CameraComponentEditFactory.generated.h"
#include "generated/CollisionComponentEditFactory.generated.h"
#include "generated/LightComponentEditFactory.generated.h"
#include "generated/MovementComponentEditFactory.generated.h"
#include "generated/NavMeshComponentEditFactory.generated.h"
#include "generated/RenderComponentEditFactory.generated.h"
#include "generated/RenderModeComponentEditFactory.generated.h"
#include "generated/TransformComponentEditFactory.generated.h"

#include "GameData/Components/AiControllerComponent.generated.h"
#include "GameData/Components/CameraComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/LightComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/NavMeshComponent.generated.h"
#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/RenderModeComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"

void ComponentRegistration::RegisterToEditFactory(std::map<std::string, std::unique_ptr<AbstractEditFactory>>& factories)
{
	factories.try_emplace(AiControllerComponent::GetClassName(), std::make_unique<AiControllerComponentEditFactory>());
	factories.try_emplace(CameraComponent::GetClassName(), std::make_unique<CameraComponentEditFactory>());
	factories.try_emplace(CollisionComponent::GetClassName(), std::make_unique<CollisionComponentEditFactory>());
	factories.try_emplace(LightComponent::GetClassName(), std::make_unique<LightComponentEditFactory>());
	factories.try_emplace(MovementComponent::GetClassName(), std::make_unique<MovementComponentEditFactory>());
	factories.try_emplace(NavMeshComponent::GetClassName(), std::make_unique<NavMeshComponentEditFactory>());
	factories.try_emplace(RenderComponent::GetClassName(), std::make_unique<RenderComponentEditFactory>());
	factories.try_emplace(RenderModeComponent::GetClassName(), std::make_unique<RenderModeComponentEditFactory>());
	factories.try_emplace(TransformComponent::GetClassName(), std::make_unique<TransformComponentEditFactory>());
}

void ComponentRegistration::RegisterComponentFactory(ComponentFactory& factory)
{
	factory.registerComponent<AiControllerComponent>();
	factory.registerComponent<CameraComponent>();
	factory.registerComponent<CollisionComponent>();
	factory.registerComponent<LightComponent>();
	factory.registerComponent<MovementComponent>();
	factory.registerComponent<NavMeshComponent>();
	factory.registerComponent<RenderComponent>();
	factory.registerComponent<RenderModeComponent>();
	factory.registerComponent<TransformComponent>();
}
