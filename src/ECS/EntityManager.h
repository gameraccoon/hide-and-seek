#pragma once

#include <tuple>
#include <typeindex>
#include <unordered_map>

#include "Base/Types/TemplateAliases.h"

#include "ECS/Component.h"
#include "ECS/Entity.h"
#include "ECS/Delegates.h"

class ComponentFactory;

class EntityManager
{
public:
	EntityManager() = default;
	~EntityManager();

	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;
	EntityManager(EntityManager&&) = delete;
	EntityManager& operator=(EntityManager&&) = delete;

	Entity addEntity();
	void removeEntity(Entity entity);
	std::unordered_map<Entity::EntityID, size_t>& getEntities() { return mEntityIndexMap; }

	// these two should be used carefully (added for the editor)
	Entity getNonExistentEntity();
	void insertEntityUnsafe(Entity entity);

	std::vector<BaseComponent*> getAllEntityComponents(Entity entity);

	template<typename ComponentType>
	bool doesEntityHaveComponent(Entity entity)
	{
		auto entityIdxItr = mEntityIndexMap.find(std::forward<Entity>(entity).getID());
		if (entityIdxItr == mEntityIndexMap.end())
		{
			return false;
		}

		auto& componentVector = mComponents[typeid(ComponentType)];

		return entityIdxItr->second < componentVector.size() && componentVector[entityIdxItr->second] != nullptr;
	}

	template<typename ComponentType>
	ComponentType* addComponent(Entity entity)
	{
		auto entityIdxItr = mEntityIndexMap.find(std::forward<Entity>(entity).getID());
		if (entityIdxItr == mEntityIndexMap.end())
		{
			return nullptr;
		}

		ComponentType* component = new ComponentType();

		addComponentToEntity(entityIdxItr->second, component, typeid(ComponentType));

		return component;
	}

	void addComponent(Entity entity, BaseComponent* component, std::type_index typeID);

	template<typename ComponentType>
	void removeComponent(Entity entity)
	{
		removeComponent(std::forward<Entity>(entity), typeid(ComponentType));
	}

	void removeComponent(Entity entity, std::type_index typeID);

	template<typename ComponentType>
	ComponentType* scheduleAddComponent(Entity entity)
	{
		ComponentType* component = new ComponentType();
		scheduleAddComponentToEntity(entity, component, typeid(ComponentType));
		return component;
	}

	void scheduleAddComponentToEntity(Entity entity, BaseComponent* component, std::type_index typeID);

	template<typename ComponentType>
	void scheduleRemoveComponent(Entity entity)
	{
		scheduleRemoveComponent(std::forward<Entity>(entity), typeid(ComponentType));
	}

	void scheduleRemoveComponent(Entity entity, std::type_index typeID);
	void executeScheduledActions();

	template<typename... Components>
	std::tuple<Components*...> getEntityComponents(Entity entity)
	{
		auto entityIdxItr = mEntityIndexMap.find(entity.getID());
		if (entityIdxItr == mEntityIndexMap.end())
		{
			return std::make_tuple(static_cast<Components*>(nullptr)...);
		}
		EntityIndex entityIdx = entityIdxItr->second;

		auto componentVectors = std::make_tuple(mComponents[typeid(Components)]...);

		return getEntityComponentSet<Components...>(entityIdx, componentVectors);
	}

	template<typename FirstComponent, typename... Components, typename... AdditionalData>
	void getComponents(TupleVector<FirstComponent*, AdditionalData..., Components*...>& inOutComponents, AdditionalData... data)
	{
		auto& firstComponentVector = mComponents[typeid(FirstComponent)];

		auto componentVectors = std::make_tuple(firstComponentVector, mComponents[typeid(Components)]...);

		constexpr unsigned componentsSize = sizeof...(Components);

		for (EntityIndex entityIndex = 0, i_size = firstComponentVector.size(); entityIndex < i_size; ++entityIndex)
		{
			auto& firstComponent = firstComponentVector[entityIndex];
			if (firstComponent == nullptr)
			{
				continue;
			}

			auto components = getEntityComponentSet<FirstComponent, Components...>(entityIndex, componentVectors);

			if (std::get<componentsSize>(components) != nullptr)
			{
				inOutComponents.push_back(std::tuple_cat(std::make_tuple(data...), std::move(components)));
			}
		}
	}

	template<typename FirstComponent, typename... Components, typename... AdditionalData>
	void getComponentsWithEntities(TupleVector<Entity, AdditionalData..., FirstComponent*, Components*...>& inOutComponents, AdditionalData... data)
	{
		auto& firstComponentVector = mComponents[typeid(FirstComponent)];

		auto componentVectors = std::make_tuple(firstComponentVector, mComponents[typeid(Components)]...);

		constexpr unsigned componentsSize = sizeof...(Components);

		for (auto& [entityID, entityIndex] : mEntityIndexMap)
		{
			if (entityIndex >= firstComponentVector.size())
			{
				continue;
			}

			auto& firstComponent = firstComponentVector[entityIndex];
			if (firstComponent == nullptr)
			{
				continue;
			}

			auto components = getEntityComponentSet<FirstComponent, Components...>(entityIndex, componentVectors);

			if (std::get<componentsSize>(components) != nullptr)
			{
				inOutComponents.push_back(std::tuple_cat(std::make_tuple(Entity(entityID)), std::make_tuple(data...), std::move(components)));
			}
		}
	}

	template<typename FirstComponent, typename... Components, typename FunctionType, typename... AdditionalData>
	void forEachComponentSet(FunctionType processor, AdditionalData... data)
	{
		auto& firstComponentVector = mComponents[typeid(FirstComponent)];

		auto componentVectors = std::make_tuple(firstComponentVector, mComponents[typeid(Components)]...);

		constexpr unsigned componentsSize = sizeof...(Components);

		for (EntityIndex entityIndex = 0, i_size = firstComponentVector.size(); entityIndex < i_size; ++entityIndex)
		{
			auto& firstComponent = firstComponentVector[entityIndex];
			if (firstComponent == nullptr)
			{
				continue;
			}

			auto components = getEntityComponentSet<FirstComponent, Components...>(entityIndex, componentVectors);

			if (std::get<componentsSize>(components) == nullptr)
			{
				continue;
			}

			std::apply(processor, std::tuple_cat(std::make_tuple(data...), std::move(components)));
		}
	}

	template<typename FirstComponent, typename... Components, typename FunctionType, typename... AdditionalData>
	void forEachComponentSetWithEntity(FunctionType processor, AdditionalData... data)
	{
		auto& firstComponentVector = mComponents[typeid(FirstComponent)];

		auto componentVectors = std::make_tuple(firstComponentVector, mComponents[typeid(Components)]...);

		constexpr unsigned componentsSize = sizeof...(Components);

		for (auto& [entityID, entityIndex] : mEntityIndexMap)
		{
			if (entityIndex >= firstComponentVector.size())
			{
				continue;
			}

			auto& firstComponent = firstComponentVector[entityIndex];
			if (firstComponent == nullptr)
			{
				continue;
			}

			auto components = getEntityComponentSet<FirstComponent, Components...>(entityIndex, componentVectors);

			if (std::get<componentsSize>(components) == nullptr)
			{
				continue;
			}

			std::apply(processor, std::tuple_cat(std::make_tuple(Entity(entityID)), std::make_tuple(data...), std::move(components)));
		}
	}

	template<typename FirstComponent, typename... Components>
	std::vector<Entity> getEntitiesHavingComponents()
	{
		std::vector<Entity> result;
		auto& firstComponentVector = mComponents[typeid(FirstComponent)];
		result.reserve(firstComponentVector.size());

		auto componentVectors = std::make_tuple(firstComponentVector, mComponents[typeid(Components)]...);

		constexpr unsigned componentsSize = sizeof...(Components);

		for (auto& [entityID, entityIndex] : mEntityIndexMap)
		{
			if (entityIndex >= firstComponentVector.size())
			{
				continue;
			}

			auto& firstComponent = firstComponentVector[entityIndex];
			if (firstComponent == nullptr)
			{
				continue;
			}

			auto components = getEntityComponentSet<FirstComponent, Components...>(entityIndex, componentVectors);

			if (std::get<componentsSize>(components) == nullptr)
			{
				continue;
			}

			result.emplace_back(entityID);
		}
		return result;
	}

	void getPrefabFromEntity(nlohmann::json& json, Entity entity);
	Entity createPrefabInstance(const nlohmann::json& json, const ComponentFactory& componentFactory);
	void applyPrefabToExistentEntity(const nlohmann::json& json, Entity entity, const ComponentFactory& componentFactory);

	void transferEntityTo(EntityManager& otherManager, Entity entity);

	nlohmann::json toJson(const class ComponentFactory& componentFactory) const;
	void fromJson(const nlohmann::json& json, const class ComponentFactory& componentFactory);

public:
	MulticastDelegate<> OnEntityAdded;
	MulticastDelegate<> OnEntityRemoved;

private:
	using EntityIndex = size_t;

	struct ComponentToAdd
	{
		Entity entity;
		BaseComponent* component;
		std::type_index typeID;

		ComponentToAdd(Entity entity, BaseComponent* component, std::type_index typeID)
			: entity(entity),
			  component(component),
			  typeID(typeID)
		{}
	};

	struct ComponentToRemove
	{
		Entity entity;
		std::type_index typeID;

		ComponentToRemove(Entity entity, std::type_index typeID)
			: entity(entity),
			  typeID(typeID)
		{}
	};

private:
	template<int I = 0>
	std::tuple<> getEmptyComponents()
	{
		return std::tuple<>();
	}

	template<typename FirstComponent, typename... Components>
	std::tuple<FirstComponent*, Components*...> getEmptyComponents()
	{
		return std::tuple_cat(std::tuple<FirstComponent*>(nullptr), getEmptyComponents<Components...>());
	}

	template<unsigned Index, typename Datas>
	std::tuple<> getEntityComponentSetInner(EntityIndex /*entityIdx*/, Datas& /*componentVectors*/)
	{
		return std::tuple<>();
	}

	template<unsigned Index, typename Datas, typename FirstComponent, typename... Components>
	std::tuple<FirstComponent*, Components*...> getEntityComponentSetInner(EntityIndex entityIdx, Datas& componentVectors)
	{
		if (std::get<Index>(componentVectors).size() <= entityIdx)
		{
			return getEmptyComponents<FirstComponent, Components...>();
		}

		auto& component = std::get<Index>(componentVectors)[entityIdx];
		if (component == nullptr)
		{
			return getEmptyComponents<FirstComponent, Components...>();
		}

		return std::tuple_cat(std::make_tuple(static_cast<FirstComponent*>(component)), getEntityComponentSetInner<Index + 1, Datas, Components...>(entityIdx, componentVectors));
	}

	template<typename FirstComponent, typename... Components, typename... Data>
	std::tuple<FirstComponent*, Components*...> getEntityComponentSet(EntityIndex entityIdx, std::tuple<std::vector<Data*>...>& componentVectors)
	{
		using Datas = std::tuple<std::vector<Data*>...>;
		return getEntityComponentSetInner<0, Datas, FirstComponent, Components...>(entityIdx, componentVectors);
	}

	void addComponentToEntity(EntityIndex entityIdx, BaseComponent* component, std::type_index typeID);

private:
	std::unordered_map<std::type_index, std::vector<BaseComponent*>> mComponents;
	std::unordered_map<Entity::EntityID, EntityIndex> mEntityIndexMap;
	std::unordered_map<EntityIndex, Entity::EntityID> mIndexEntityMap;

	std::vector<ComponentToAdd> mScheduledComponentAdditions;
	std::vector<ComponentToRemove> mScheduledComponentRemovements;

	EntityIndex mNextEntityIndex = 0;
};
