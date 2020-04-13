#include "Base/precomp.h"

#include "ECS/ComponentSetHolder.h"

#include <nlohmann/json.hpp>

#include "ComponentFactory.h"

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
	}
}

void ComponentSetHolder::removeComponent(std::type_index typeID)
{
	if (auto it = mComponents.find(typeID); it != mComponents.end())
	{
		delete it->second;
		mComponents.erase(it);
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
		components[ID_TO_STR(componentFactory.getStringFromTypeID(component.first))] = componenObj;
	}
	outJson["components"] = components;

	return outJson;
}

void ComponentSetHolder::fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	const auto& components = json.at("components");
	for (const auto& [stringType, componentData] : components.items())
	{
		StringID type = STR_TO_ID(stringType);
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
