#include "componentregistration.h"

#include "transformcomponenteditfactory.h"

#include <Components/CameraComponent.h>
#include <Components/CollisionComponent.h>
#include <Components/LightComponent.h>
#include <Components/MovementComponent.h>
#include <Components/RenderComponent.h>
#include <Components/TransformComponent.h>

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
