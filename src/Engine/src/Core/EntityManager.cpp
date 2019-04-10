#include "Core/EntityManager.h"

#include <algorithm>
#include <random>

#include <nlohmann/json.hpp>

#include <Modules/ComponentFactory.h>

Entity EntityManager::addEntity()
{
	// ToDo: use generators
	Entity::EntityID id = static_cast<Entity::EntityID>(std::rand());
	mEntityIndexMap.emplace(id, mMaxEntityIndex++);
	return Entity(id);
}

void EntityManager::removeEntity(Entity entity)
{
	auto entityIdxItr = mEntityIndexMap.find(entity.getID());
	if (entityIdxItr == mEntityIndexMap.end())
	{
		return;
	}
	EntityIndex oldEntityIdx = entityIdxItr->second;

	for (auto& componentVector : mComponents)
	{
		// if the vector containts deleted entity
		if (componentVector.second.size() > oldEntityIdx)
		{
			// if the vector containts the last entity
			if (componentVector.second.size() < mMaxEntityIndex - 1)
			{
				// swap with the last and drop it
				std::swap(componentVector.second[oldEntityIdx], componentVector.second[mMaxEntityIndex - 1]);
				delete componentVector.second[mMaxEntityIndex - 1];
				componentVector.second.pop_back();
			}
			else
			{
				// emulate swapping with nonexsistent nullptr Component
				delete componentVector.second[oldEntityIdx];
				componentVector.second[oldEntityIdx] = nullptr;
			}
		}
	}

	--mMaxEntityIndex;

	mEntityIndexMap.erase(entity.getID());

	// relink maps
	Entity::EntityID entityID = mIndexEntityMap[mMaxEntityIndex];
	mEntityIndexMap[entityID] = oldEntityIdx;
	mIndexEntityMap[oldEntityIdx] = entityID;
	mIndexEntityMap.erase(mMaxEntityIndex);
}

nlohmann::json EntityManager::toJson(const ComponentFactory& componentFactory) const
{
	nlohmann::json outJson{
		{"entityIndexMap", mEntityIndexMap}
	};

	auto components = nlohmann::json{};

	for (auto& componentArray : mComponents)
	{
		auto componentArrayObject = nlohmann::json::array();
		for (auto& component : componentArray.second)
		{
			auto componenObj = nlohmann::json{};
			if (component != nullptr)
			{
				component->toJson(componenObj);
			}
			componentArrayObject.push_back(componenObj);
		}
		components[componentFactory.getStringFromTypeID(componentArray.first)] = componentArrayObject;
	}
	outJson["components"] = components;

	return outJson;
}

void EntityManager::fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	json.at("entityIndexMap").get_to(mEntityIndexMap);

	for (const auto& item : mEntityIndexMap)
	{
		mIndexEntityMap[item.second] = item.first;
	}

	const auto& components = json.at("components");
	for (auto& [type, vector] : components.items())
	{
		std::optional<std::type_index> typeIndex = componentFactory.getTypeIDFromString(type);
		ComponentFactory::DeserializationFn deserializator = componentFactory.getDeserializator(type);
		if (typeIndex.has_value() && deserializator != nullptr)
		{
			std::vector<BaseComponent*>& componentsVector = mComponents[typeIndex.value()];
			componentsVector.reserve(vector.size());
			for (const auto& component : vector)
			{
				if (!component.is_null())
				{
					componentsVector.push_back(deserializator(component));
				}
				else
				{
					componentsVector.push_back(nullptr);
				}
			}
		}
	}
}
