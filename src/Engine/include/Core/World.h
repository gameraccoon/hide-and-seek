#pragma once

#include <string>
#include <tuple>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <functional>

#include "Component.h"
#include "Entity.h"
#include "Structures/PathPoint.h"

class World
{
public:
	World() = default;
	~World();

	Entity addEntity();
	void removeEntity(Entity entity);

	template<typename T, typename... Args>
	std::shared_ptr<T> addComponent(const Entity& entity, Args&&... args)
	{
		auto entityIdxItr = mEntityIndexMap.find(entity.getID());
		if (entityIdxItr == mEntityIndexMap.end())
		{
			return nullptr;
		}
		EntityIndex entityIdx = entityIdxItr->second;

		std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(args)...);
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
	std::tuple<std::shared_ptr<Components>...> getEntityComponents(const Entity& entity)
	{
		auto entityIdxItr = mEntityIndexMap.find(entity.getID());
		if (entityIdxItr == mEntityIndexMap.end())
		{
			return std::make_tuple(std::shared_ptr<Components>(nullptr)...);
		}
		EntityIndex entityIdx = entityIdxItr->second;

		auto componentVectors = std::make_tuple(mComponents[typeid(Components)]...);

		return getEntityComponentSet<Components...>(entityIdx, componentVectors);
	}

	template<typename FirstComponent, typename... Components>
	std::vector<std::tuple<std::shared_ptr<FirstComponent>, std::shared_ptr<Components>...>> getComponents()
	{
		auto& firstComponentVector = mComponents[typeid(FirstComponent)];

		std::vector<std::tuple<std::shared_ptr<FirstComponent>, std::shared_ptr<Components>...>> result;
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

			if (std::get<componentsSize - 1>(components) != nullptr)
			{
				result.push_back(components);
			}
		}

		return result;
	}

	template<typename FirstComponent, typename... Components>
	void forEachEntity(typename std::function<bool(std::tuple<std::shared_ptr<FirstComponent>, std::shared_ptr<Components>...>)> processor)
	{
		auto& firstComponentVector = mComponents[typeid(FirstComponent)];

		decltype(auto) componentVectors = std::make_tuple(firstComponentVector, mComponents[typeid(Components)]...);

		constexpr unsigned componentsSize = sizeof...(Components) + 1;

		for (EntityIndex entityIndex = 0, i_size = firstComponentVector.size(); entityIndex < i_size; ++entityIndex)
		{
			auto& firstComponent = firstComponentVector[entityIndex];
			if (firstComponent == nullptr)
			{
				continue;
			}

			auto components = getEntityComponentSet<FirstComponent, Components...>(entityIndex, componentVectors);

			if (std::get<componentsSize - 1>(components) == nullptr)
			{
				continue;
			}

			bool isGood = processor(components);
			if (!isGood)
			{
				break;
			}
		}
	}

	NullableEntity getMainCamera() { return mMainCamera; }
	void setMainCamera(const NullableEntity& newCamera) { mMainCamera = newCamera; }

	NullableEntity getPlayerControlledEntity() { return mPlayerControlledEntity; }
	void setPlayerControlledEntity(const NullableEntity& newEntity) { mPlayerControlledEntity = newEntity; }

	/** Add a new PathPoint to the navigation map */
	void addPathPoint(PathPoint::Ptr& pathPoint);
	const std::vector<PathPoint::Ptr>& getNavigationMap() const;

private:
	typedef size_t EntityIndex;

private:
	template<int I = 0>
	std::tuple<> getEmptyComponents()
	{
		return std::tuple<>();
	}

	template<typename FirstComponent, typename... Components>
	std::tuple<std::shared_ptr<FirstComponent>, std::shared_ptr<Components>...> getEmptyComponents()
	{
		return std::tuple_cat(std::tuple<std::shared_ptr<FirstComponent>>(nullptr), getEmptyComponents<Components...>());
	}

	template<unsigned Index, typename Datas>
	std::tuple<> getEntityComponentSetInner(EntityIndex /*entityIdx*/, Datas& /*componentVectors*/)
	{
		return std::tuple<>();
	}

	template<unsigned Index, typename Datas, typename FirstComponent, typename... Components>
	std::tuple<std::shared_ptr<FirstComponent>, std::shared_ptr<Components>...> getEntityComponentSetInner(EntityIndex entityIdx, Datas& componentVectors)
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

		return std::tuple_cat(std::make_tuple(std::dynamic_pointer_cast<FirstComponent>(component)), getEntityComponentSetInner<Index + 1, Datas, Components...>(entityIdx, componentVectors));
	}

	template<typename FirstComponent, typename... Components, typename... Data>
	std::tuple<std::shared_ptr<FirstComponent>, std::shared_ptr<Components>...> getEntityComponentSet(EntityIndex entityIdx, std::tuple<std::vector<std::shared_ptr<Data>>...>& componentVectors)
	{
		using Datas = std::tuple<std::vector<std::shared_ptr<Data>>...>;
		return getEntityComponentSetInner<0, Datas, FirstComponent, Components...>(entityIdx, componentVectors);
	}

private:
	/** Destroy actors that are waiting to be destroyed */
	void cleanDestroyedActors();
	/** All pathpoints of this world */
	std::vector<PathPoint::Ptr> mNavigationMap;

	NullableEntity mMainCamera;
	NullableEntity mPlayerControlledEntity;

	std::unordered_map<std::type_index, std::vector<BaseComponent::Ptr>> mComponents;
	std::unordered_map<Entity::EntityID, EntityIndex> mEntityIndexMap;
	std::unordered_map<EntityIndex, Entity::EntityID> mIndexEntityMap;

	unsigned int mMaxEntityIndex = 0;
};
