#pragma once

#include <map>
#include <string>
#include <functional>
#include <optional>

#include <nlohmann/json_fwd.hpp>

#include <Core/Component.h>
#include <typeindex>

class ComponentFactory
{
public:
	typedef std::function<BaseComponent*(const nlohmann::json&)> DeserializationFn;

	template<typename T>
	void registerComponent()
	{
		mComponentDeserializators[T::GetClassName()] = [](const nlohmann::json& json) {
			T* result = new T();
			from_json(json, *result);
			return result;
		};
		std::string className = T::GetClassName();
		mStringToTypeID.emplace(className, typeid(T));
		mTypeIDToString.emplace(typeid(T), className);
	}

	DeserializationFn getDeserializator(const std::string& type) const;
	std::optional<std::type_index> getTypeIDFromString(const std::string& type) const;
	std::string getStringFromTypeID(const std::type_index& typeID) const;

private:
	std::map<std::string, DeserializationFn> mComponentDeserializators;
	std::map<std::string, std::type_index> mStringToTypeID;
	std::map<std::type_index, std::string> mTypeIDToString;
};
