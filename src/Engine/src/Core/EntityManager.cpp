#include "Core/EntityManager.h"

#include <algorithm>
#include <random>

#include <nlohmann/json.hpp>

#include <Modules/ComponentFactory.h>
#include "Debug/Assert.h"

static const int EntityInsertionTrialsLimit = 10;

Entity EntityManager::addEntity()
{
	int insertionTrial = 0;

	while (insertionTrial < EntityInsertionTrialsLimit)
	{
		// ToDo: use generators
		Entity::EntityID id = static_cast<Entity::EntityID>(std::rand());
		auto insertionResult = mEntityIndexMap.try_emplace(id, mNextEntityIndex);
		if (insertionResult.second)
		{
			mIndexEntityMap.emplace(mNextEntityIndex, id);
			++mNextEntityIndex;
			return Entity(id);
		}
		++insertionTrial;
	}

	//ReportFailure("Can't generate unique ID for an entity");
	return Entity(0);
}

void EntityManager::removeEntity(Entity entity)
{
	auto entityIdxItr = mEntityIndexMap.find(entity.getID());
	if (entityIdxItr == mEntityIndexMap.end())
	{
		return;
	}
	EntityIndex oldEntityIdx = entityIdxItr->second;

	--mNextEntityIndex; // now it points to the element that going to be removed

	for (auto& componentVector : mComponents)
	{
		// if the vector containts deleted entity
		if (oldEntityIdx < componentVector.second.size())
		{
			// remove the element
			delete componentVector.second[oldEntityIdx];
			componentVector.second[oldEntityIdx] = nullptr;

			// if the vector containts the last entity
			if (mNextEntityIndex < componentVector.second.size() && oldEntityIdx != mNextEntityIndex)
			{
				// move it to the freed space
				std::swap(componentVector.second[oldEntityIdx], componentVector.second[mNextEntityIndex]);
			}
		}
	}

	mEntityIndexMap.erase(entity.getID());

	if (oldEntityIdx != mNextEntityIndex)
	{
		// relink maps
		Entity::EntityID entityID = mIndexEntityMap[mNextEntityIndex];
		mEntityIndexMap[entityID] = oldEntityIdx;
		mIndexEntityMap[oldEntityIdx] = entityID;
	}
	mIndexEntityMap.erase(mNextEntityIndex);
}

std::vector<BaseComponent*> EntityManager::getAllEntityComponents(const Entity& entity)
{
	std::vector<BaseComponent*> components;
	auto entityIdxItr = mEntityIndexMap.find(entity.getID());
	if (entityIdxItr != mEntityIndexMap.end())
	{
		EntityIndex index = entityIdxItr->second;
		for (auto& componentArray : mComponents)
		{
			if (componentArray.second.size() > index && componentArray.second[index] != nullptr)
			{
				components.push_back(componentArray.second[index]);
			}
		}
	}
	return components;
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

	auto maxElementIt = std::max_element(mIndexEntityMap.begin(), mIndexEntityMap.end());
	if (maxElementIt != mIndexEntityMap.end())
	{
		mNextEntityIndex = maxElementIt->first + 1;
	}
	else
	{
		mNextEntityIndex = 0;
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
