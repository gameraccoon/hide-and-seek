#include "componentregistration.h"

#include "generated/transformcomponenteditfactory.generated.h"

#include <Components/CameraComponent.generated.h>
#include <Components/CollisionComponent.generated.h>
#include <Components/LightComponent.generated.h>
#include <Components/MovementComponent.generated.h>
#include <Components/RenderComponent.generated.h>
#include <Components/TransformComponent.generated.h>

#include <Modules/ComponentFactory.h>

void ComponentRegistration::RegisterToEditFactory(std::map<std::string, std::unique_ptr<AbstractEditFactory>>& factories)
{
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
