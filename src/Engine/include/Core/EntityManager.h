#pragma once

#include <tuple>
#include <typeindex>
#include <unordered_map>

#include "Component.h"
#include "Entity.h"

class EntityManager
{
public:
	Entity addEntity();
	void removeEntity(Entity entity);
	std::unordered_map<Entity::EntityID, size_t>& getEntities() { return mEntityIndexMap; }

	std::vector<BaseComponent*> getAllEntityComponents(const Entity& entity);

	template<typename T>
	T* addComponent(const Entity& entity)
	{
		auto entityIdxItr = mEntityIndexMap.find(entity.getID());
		if (entityIdxItr == mEntityIndexMap.end())
		{
			return nullptr;
		}
		EntityIndex entityIdx = entityIdxItr->second;

		T* component = new T();
		auto& componentsVector = mComponents[typeid(T)];
		if (componentsVector.size() <= entityIdx)
		{
			if (componentsVector.capacity() <= entityIdx)
			{
				componentsVector.reserve((entityIdx + 1) * 2);
			}
			componentsVector.resize(entityIdx + 1);
		}
		componentsVector[entityIdx] = component;
		return component;
	}

	template<typename... Components>
	std::tuple<Components*...> getEntityComponents(const Entity& entity)
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

	template<typename FirstComponent, typename... Components>
	std::vector<std::tuple<FirstComponent*, Components*...>> getComponents()
	{
		auto& firstComponentVector = mComponents[typeid(FirstComponent)];

		std::vector<std::tuple<FirstComponent*, Components*...>> result;
		result.reserve(firstComponentVector.size());

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
				result.push_back(components);
			}
		}

		return result;
	}

	template<typename FirstComponent, typename... Components, typename FunctionType>
	void forEachEntity(FunctionType processor)
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

			std::apply(processor, components);
		}
	}

	nlohmann::json toJson(const class ComponentFactory& componentFactory) const;
	void fromJson(const nlohmann::json& json, const class ComponentFactory& componentFactory);

private:
	typedef size_t EntityIndex;

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

private:
	std::unordered_map<std::type_index, std::vector<BaseComponent*>> mComponents;
	std::unordered_map<Entity::EntityID, EntityIndex> mEntityIndexMap;
	std::unordered_map<EntityIndex, Entity::EntityID> mIndexEntityMap;

	EntityIndex mNextEntityIndex = 0;
};
