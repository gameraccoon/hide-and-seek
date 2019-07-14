#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <any>
#include <typeindex>
#include <functional>
#include <memory>

#include "Debug/Assert.h"

namespace FSM
{
	class Blackboard
	{
	public:
		using KeyType = std::string;
	public:
		template <typename T>
		void setValue(KeyType key, T value)
		{
			mValues.insert_or_assign(std::forward<KeyType>(key), std::forward<T>(value));
		}

		template <typename T>
		T getValue(KeyType key) const
		{
			auto it = mValues.find(std::forward<KeyType>(key));
			if (it != mValues.end())
			{
				AssertFatal(it->second.type() == typeid(T), "Blackboard wrong type requested: %s", it->second.type().name());
				return std::any_cast<T>(it->second);
			}

			return T();
		}

		template <typename T>
		T getValue(KeyType key, std::common_type_t<T> def) const
		{
			auto it = mValues.find(std::forward<KeyType>(key));
			if (it != mValues.end())
			{
				AssertFatal(it->second.type() == typeid(T), "Blackboard wrong type requested: %s", it->second.type().name());
				return std::any_cast<T>(it->second);
			}

			return std::forward<T>(def);
		}

	private:
		std::unordered_map<std::string, std::any> mValues;
	};
}
