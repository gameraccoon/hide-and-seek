#pragma once

#include <unordered_map>
#include <algorithm>

#include "ECS/Component.h"
#include "Base/Types/String/StringID.h"

class ComponentMap
{
public:
	using Iterator = std::unordered_map<StringID, std::vector<BaseComponent*>>::iterator;
	using ConstIterator = std::unordered_map<StringID, std::vector<BaseComponent*>>::const_iterator;

public:
	ComponentMap() = default;
	ComponentMap(const ComponentMap&) = delete;
	ComponentMap& operator=(const ComponentMap&) = delete;
	ComponentMap(ComponentMap&&) = delete;
	ComponentMap& operator=(ComponentMap&&) = delete;
	~ComponentMap() { AssertFatal(mEmptyVector.empty(), "mEmptyVector has changed during runtime, that should never happen"); }

	template<typename FirstComponent, typename... Components>
	[[nodiscard]] auto getComponentVectors()
	{
		auto it = mData.find(FirstComponent::GetTypeName());

		if (it == mData.end())
		{
			return std::tuple_cat(std::tuple<std::vector<BaseComponent*>&>(mEmptyVector), getEmptyComponentVectors<Components>()...);
		}

		return std::tuple_cat(std::tuple<std::vector<BaseComponent*>&>(it->second), getComponentVectors<Components>()...);
	}

	[[nodiscard]] std::vector<BaseComponent*>& getComponentVectorByID(StringID id)
	{
		auto it = mData.find(id);
		return it == mData.end() ? mEmptyVector : it->second;
	}

	[[nodiscard]] const std::vector<BaseComponent*>& getComponentVectorByID(StringID id) const
	{
		return const_cast<ComponentMap*>(this)->getComponentVectorByID(id);
	}

	[[nodiscard]] std::vector<BaseComponent*>& getOrCreateComponentVectorByID(StringID id)
	{
		return mData[id];
	}

	void cleanEmptyVetors()
	{
		for (auto it = mData.begin(), itEnd = mData.end(); it != itEnd;)
		{
			if (it->second.empty())
			{
				it = mData.erase(it);
			}
			else
			{
				++it;
			}
		}
		AssertFatal(mEmptyVector.empty(), "mEmptyVector should be empty");
	}

	[[nodiscard]] Iterator begin() noexcept { return mData.begin(); }
	[[nodiscard]] Iterator end() noexcept { return mData.end(); }
	[[nodiscard]] ConstIterator begin() const noexcept { return mData.cbegin(); }
	[[nodiscard]] ConstIterator end() const noexcept { return mData.cend(); }

private:
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

private:
	std::unordered_map<StringID, std::vector<BaseComponent*>> mData;
	std::vector<BaseComponent*> mEmptyVector;
};
