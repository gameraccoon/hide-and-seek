#pragma once

#include <unordered_map>
#include <algorithm>

#include "ECS/Component.h"
#include "Base/Types/String/StringID.h"

class ComponentMap
{
public:
	ComponentMap() = default;
	ComponentMap(const ComponentMap&) = delete;
	ComponentMap& operator=(const ComponentMap&) = delete;
	ComponentMap(ComponentMap&&) = delete;
	ComponentMap& operator=(ComponentMap&&) = delete;
	~ComponentMap() { AssertFatal(mEmptyVector.empty(), "mEmptyVector has changed during runtime, that should never happen"); }

	class Iterator
	{
	public:
		Iterator(ComponentMap& container, size_t idx)
			: mContainer(container)
			, mIdx(idx)
		{}

		using ValueType = std::pair<StringID, std::vector<BaseComponent*>&>;

		Iterator& operator++() { ++mIdx; return (*this); }
		ValueType operator*() { return ValueType(mContainer.mKeys[mIdx], mContainer.mValues[mIdx]); }
		ValueType operator*() const { return ValueType(mContainer.mKeys[mIdx], mContainer.mValues[mIdx]); }
		// assumes that you will never compire iterators from different maps
		bool operator!=(const Iterator& other) const { return mIdx != other.mIdx; }

	private:
		ComponentMap& mContainer;
		size_t mIdx;
	};

	class ConstIterator
	{
	public:
		ConstIterator(const ComponentMap& container, size_t idx)
			: mContainer(container)
			, mIdx(idx)
		{}

		using ValueType = std::pair<StringID, const std::vector<BaseComponent*>&>;

		ConstIterator& operator++() { ++mIdx; return (*this); }
		ValueType operator*() const { return ValueType(mContainer.mKeys[mIdx], mContainer.mValues[mIdx]); }
		// assumes that you will never compire iterators from different maps
		bool operator!=(const ConstIterator& other) const { return mIdx != other.mIdx; }

	private:
		const ComponentMap& mContainer;
		size_t mIdx;
	};

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
		size_t idx = getKeyByID(FirstComponent::GetTypeName());

		if (idx == InvalidIndex)
		{
			return std::tuple_cat(std::tuple<std::vector<BaseComponent*>&>(mEmptyVector), getEmptyComponentVectors<Components>()...);
		}

		return std::tuple_cat(std::tuple<std::vector<BaseComponent*>&>(mValues[idx]), getComponentVectors<Components>()...);
	}

	std::vector<BaseComponent*>& getComponentVectorByID(StringID id)
	{
		size_t idx = getKeyByID(id);
		return idx == InvalidIndex ? mEmptyVector : mValues[idx];
	}

	const std::vector<BaseComponent*>& getComponentVectorByID(StringID id) const
	{
		return const_cast<ComponentMap*>(this)->getComponentVectorByID(id);
	}

	std::vector<BaseComponent*>& getOrCreateComponentVectorByID(StringID id)
	{
		size_t idx = getKeyByID(id);
		return idx == InvalidIndex ? createNewComponentVector(id) : mValues[idx];
	}

	Iterator begin() { return Iterator(*this, 0); }
	Iterator end() { return Iterator(*this, mKeys.size()); }
	ConstIterator begin() const { return ConstIterator(*this, 0); }
	ConstIterator end() const { return ConstIterator(*this, mKeys.size()); }

private:
	size_t getKeyByID(StringID id) const
	{
		auto it = std::find(mKeys.begin(), mKeys.end(), id);
		return it == mKeys.end() ? InvalidIndex : std::distance(mKeys.begin(), it);
	}

	std::vector<BaseComponent*>& createNewComponentVector(StringID id)
	{
		mKeys.emplace_back(id);
		mValues.emplace_back();
		AssertFatal(mKeys.size() == mValues.size(), "Keys and Values size mismatch");
		return mValues.back();
	}

private:
	constexpr static size_t InvalidIndex = std::numeric_limits<size_t>::max();

	std::vector<StringID> mKeys;
	std::vector<std::vector<BaseComponent*>> mValues;
	std::vector<BaseComponent*> mEmptyVector;
};
