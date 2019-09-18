#include "EntityManager.h"

#include <algorithm>
#include <random>

#include <nlohmann/json.hpp>

#include "ComponentFactory.h"
#include "Debug/Assert.h"
#include "Debug/Log.h"

static const int EntityInsertionTrialsLimit = 10;

EntityManager::~EntityManager()
{
	for (auto& componentVector : mComponents)
	{
		for (auto component : componentVector.second)
		{
			delete component;
		}
	}
}

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
			OnEntityAdded.broadcast();
			return Entity(id);
		}
		++insertionTrial;
	}

	ReportError("Can't generate unique ID for an entity");
	return Entity(0);
}

Entity EntityManager::getNonExistentEntity()
{
	int generationTrial = 0;

	while (generationTrial < EntityInsertionTrialsLimit)
	{
		// ToDo: use generators
		Entity::EntityID id = static_cast<Entity::EntityID>(std::rand());
		if (mEntityIndexMap.find(id) == mEntityIndexMap.end())
		{
			return Entity(id);
		}
		++generationTrial;
	}

	ReportError("Can't generate unique ID for an entity");
	return Entity(0);
}

void EntityManager::insertEntityUnsafe(Entity entity)
{
	mEntityIndexMap.emplace(entity.getID(), mNextEntityIndex);
	mIndexEntityMap.emplace(mNextEntityIndex, entity.getID());
	++mNextEntityIndex;

	OnEntityAdded.broadcast();
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

	OnEntityRemoved.broadcast();
}

std::vector<BaseComponent*> EntityManager::getAllEntityComponents(Entity entity)
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

void EntityManager::addComponent(Entity entity, BaseComponent* component, std::type_index typeID)
{
	auto entityIdxItr = mEntityIndexMap.find(entity.getID());
	if (entityIdxItr == mEntityIndexMap.end())
	{
		return;
	}

	addComponentToEntity(entityIdxItr->second, component, typeID);
}

void EntityManager::removeComponent(Entity entity, std::type_index typeID)
{
	auto entityIdxItr = mEntityIndexMap.find(entity.getID());
	if (entityIdxItr == mEntityIndexMap.end())
	{
		return;
	}

	auto& componentsVector = mComponents[typeID];

	if (componentsVector.size() > entityIdxItr->second)
	{
		delete componentsVector[entityIdxItr->second];
		componentsVector[entityIdxItr->second] = nullptr;

		OnComponentRemoved.broadcast();
	}
}

void EntityManager::scheduleAddComponentToEntity(Entity entity, BaseComponent* component, std::type_index typeID)
{
	mScheduledComponentAdditions.emplace_back(entity, component, typeID);
}

void EntityManager::scheduleRemoveComponent(Entity entity, std::type_index typeID)
{
	mScheduledComponentRemovements.emplace_back(entity, typeID);
}

void EntityManager::executeScheduledActions()
{
	for (const auto& addition : mScheduledComponentAdditions)
	{
		addComponent(addition.entity, addition.component, addition.typeID);
	}
	mScheduledComponentAdditions.clear();

	for (const auto& removement : mScheduledComponentRemovements)
	{
		removeComponent(removement.entity, removement.typeID);
	}
	mScheduledComponentRemovements.clear();
}

void EntityManager::getPrefabFromEntity(nlohmann::json& json, Entity entity)
{
	std::vector<BaseComponent*> components = getAllEntityComponents(entity);

	for (BaseComponent* component : components)
	{
		auto componenObj = nlohmann::json{};
		std::string componentTypeName = component->getComponentTypeName();
		component->toJson(componenObj);
		json[componentTypeName] = componenObj;
	}
}

Entity EntityManager::createPrefabInstance(const nlohmann::json& json, const ComponentFactory& componentFactory)
{
	Entity entity = addEntity();
	applyPrefabToExistentEntity(json, entity, componentFactory);
	return entity;
}

void EntityManager::applyPrefabToExistentEntity(const nlohmann::json& json, Entity entity, const ComponentFactory& componentFactory)
{
	for (const auto& [componentTypeName, componentObj] : json.items())
	{
		BaseComponent* component = componentFactory.createComponent(componentTypeName);

		component->fromJson(componentObj);

		addComponent(
			entity,
			component,
			componentFactory.getTypeIDFromString(componentTypeName).value()
		);
	}
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
	for (const auto& [type, vector] : components.items())
	{
		std::optional<std::type_index> typeIndex = componentFactory.getTypeIDFromString(type);
		ComponentFactory::CreationFn componentCreateFn = componentFactory.getCreationFn(type);
		if (typeIndex.has_value() && componentCreateFn != nullptr)
		{
			std::vector<BaseComponent*>& componentsVector = mComponents[typeIndex.value()];
			componentsVector.reserve(vector.size());
			for (const auto& componentData : vector)
			{
				if (!componentData.is_null())
				{
					BaseComponent* component = componentCreateFn();
					component->fromJson(componentData);
					componentsVector.push_back(component);
				}
				else
				{
					componentsVector.push_back(nullptr);
				}
			}
		}
	}
}

void EntityManager::addComponentToEntity(EntityIndex entityIdx, BaseComponent* component, std::type_index typeID)
{
	auto& componentsVector = mComponents[typeID];
	if (componentsVector.size() <= entityIdx)
	{
		if (componentsVector.capacity() <= entityIdx)
		{
			componentsVector.reserve((entityIdx + 1) * 2);
		}
		componentsVector.resize(entityIdx + 1);
	}

	if (componentsVector[entityIdx] == nullptr)
	{
		componentsVector[entityIdx] = component;
		OnComponentAdded.broadcast();
	}
}
