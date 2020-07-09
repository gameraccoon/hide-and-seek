#pragma once

#include <unordered_map>
#include <typeindex>

#include "ECS/Component.h"

class ComponentMap
{
public:
	ComponentMap() = default;
	ComponentMap(const ComponentMap&) = delete;
	ComponentMap& operator=(const ComponentMap&) = delete;
	ComponentMap(ComponentMap&&) = delete;
	ComponentMap& operator=(ComponentMap&&) = delete;

	template<int I = 0>
	std::tuple<> getEmptyComponentVectors()
	{
		return std::tuple<>();
	}

	template<typename FirstComponent, typename... Components>
	auto getEmptyComponentVectors()
	{
		return std::tuple_cat(std::tuple<std::vector<BaseComponent*>&>(mEmptyVector), getEmptyComponentVectors<Components...>());
	}

	template<int I = 0>
	std::tuple<> getComponentVectors()
	{
		return std::tuple<>();
	}

	template<typename FirstComponent, typename... Components>
	auto getComponentVectors()
	{
		auto it = mData.find(typeid(FirstComponent));

		if (it == mData.end())
		{
			return std::tuple_cat(std::tuple<std::vector<BaseComponent*>&>(mEmptyVector), getEmptyComponentVectors<Components>()...);
		}

		return std::tuple_cat(std::tuple<std::vector<BaseComponent*>&>(it->second), getComponentVectors<Components>()...);
	}

	std::vector<BaseComponent*>& getComponentVectorByID(std::type_index id)
	{
		auto it = mData.find(id);
		return it == mData.end() ? mEmptyVector : it->second;
	}

	const std::vector<BaseComponent*>& getComponentVectorByID(std::type_index id) const
	{
		auto it = mData.find(id);
		return it == mData.end() ? mEmptyVector : it->second;
	}

	std::vector<BaseComponent*>& getOrCreateComponentVectorByID(std::type_index id)
	{
		return mData[id];
	}

	std::unordered_map<std::type_index, std::vector<BaseComponent*>>& getRawData() { return mData; }
	const std::unordered_map<std::type_index, std::vector<BaseComponent*>>& getRawData() const { return mData; }

private:
	std::unordered_map<std::type_index, std::vector<BaseComponent*>> mData;
	std::vector<BaseComponent*> mEmptyVector;
};
