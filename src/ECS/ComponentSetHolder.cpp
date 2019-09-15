#include "ComponentSetHolder.h"

#include <nlohmann/json.hpp>

#include "ComponentFactory.h"
#include "Debug/Assert.h"
#include "Debug/Log.h"


ComponentSetHolder::~ComponentSetHolder()
{
	for (auto& component : mComponents)
	{
		delete component.second;
	}
}

std::vector<BaseComponent*> ComponentSetHolder::getAllComponents()
{
	std::vector<BaseComponent*> components;

	for (auto& componentArray : mComponents)
	{
		components.push_back(componentArray.second);
	}

	return components;
}

void ComponentSetHolder::addComponent(BaseComponent* component, std::type_index typeID)
{
	if (component != nullptr)
	{
		mComponents[typeID] = component;
		OnComponentAdded.broadcast();
	}
}

void ComponentSetHolder::removeComponent(std::type_index typeID)
{
	if (auto it = mComponents.find(typeID); it != mComponents.end())
	{
		delete it->second;
		mComponents.erase(it);
		OnComponentRemoved.broadcast();
	}
}

nlohmann::json ComponentSetHolder::toJson(const ComponentFactory& componentFactory) const
{
	nlohmann::json outJson;

	auto components = nlohmann::json{};

	for (auto component : mComponents)
	{
		auto componenObj = nlohmann::json{};
		component.second->toJson(componenObj);
		components[componentFactory.getStringFromTypeID(component.first)] = componenObj;
	}
	outJson["components"] = components;

	return outJson;
}

void ComponentSetHolder::fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	const auto& components = json.at("components");
	for (const auto& [type, componentData] : components.items())
	{
		std::optional<std::type_index> typeIndex = componentFactory.getTypeIDFromString(type);
		ComponentFactory::CreationFn componentCreateFn = componentFactory.getCreationFn(type);
		if (typeIndex.has_value() && componentCreateFn != nullptr)
		{
			if (!componentData.is_null())
			{
				BaseComponent* component = componentCreateFn();
				component->fromJson(componentData);
				mComponents[typeIndex.value()] = component;
			}
		}
	}
}
