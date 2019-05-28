#include "componentregistration.h"

#include "generated/CameraComponentEditFactory.generated.h"
#include "generated/CollisionComponentEditFactory.generated.h"
#include "generated/LightComponentEditFactory.generated.h"
#include "generated/MovementComponentEditFactory.generated.h"
#include "generated/RenderComponentEditFactory.generated.h"
#include "generated/TransformComponentEditFactory.generated.h"

#include <Components/CameraComponent.generated.h>
#include <Components/CollisionComponent.generated.h>
#include <Components/LightComponent.generated.h>
#include <Components/MovementComponent.generated.h>
#include <Components/RenderComponent.generated.h>
#include <Components/TransformComponent.generated.h>

#include <Utils/Core/ComponentFactory.h>

void ComponentRegistration::RegisterToEditFactory(std::map<std::string, std::unique_ptr<AbstractEditFactory>>& factories)
{
	factories.try_emplace(CameraComponent::GetClassName(), std::make_unique<CameraComponentEditFactory>());
	factories.try_emplace(CollisionComponent::GetClassName(), std::make_unique<CollisionComponentEditFactory>());
	factories.try_emplace(LightComponent::GetClassName(), std::make_unique<LightComponentEditFactory>());
	factories.try_emplace(MovementComponent::GetClassName(), std::make_unique<MovementComponentEditFactory>());
	factories.try_emplace(RenderComponent::GetClassName(), std::make_unique<RenderComponentEditFactory>());
	factories.try_emplace(TransformComponent::GetClassName(), std::make_unique<TransformComponentEditFactory>());
}

void ComponentRegistration::RegisterComponentFactory(ComponentFactory& factory)
{
	factory.registerComponent<CameraComponent>();
	factory.registerComponent<CollisionComponent>();
	factory.registerComponent<LightComponent>();
	factory.registerComponent<MovementComponent>();
	factory.registerComponent<RenderComponent>();
	factory.registerComponent<TransformComponent>();
}
