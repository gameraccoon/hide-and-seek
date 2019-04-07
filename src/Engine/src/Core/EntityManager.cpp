#include "Core/EntityManager.h"

#include <algorithm>
#include <random>

#include <nlohmann/json.hpp>

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

void to_json(nlohmann::json& outJson, const EntityManager& world)
{
	outJson = nlohmann::json{
		{"entityIndexMap", world.mEntityIndexMap}
	};

	auto components = nlohmann::json{};

	for (auto& componentArray : world.mComponents)
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
		components[componentArray.first.name()] = componentArrayObject;
	}
	outJson["components"] = components;
}

void from_json(const nlohmann::json& json, EntityManager& outWorld)
{
	json.at("entityIndexMap").get_to(outWorld.mEntityIndexMap);
//	json.at("components").get_to(outWorld.mComponents);
}
