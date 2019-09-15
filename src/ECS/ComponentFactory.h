#pragma once

#include <map>
#include <string>
#include <functional>
#include <optional>

#include "ECS/Component.h"
#include <typeindex>

class ComponentFactory
{
public:
	typedef std::function<BaseComponent*()> CreationFn;

	template<typename T>
	void registerComponent()
	{
		mComponentCreators[T::GetClassName()] = []{
			return new T();
		};
		std::string className = T::GetClassName();
		mStringToTypeID.emplace(className, typeid(T));
		mTypeIDToString.emplace(typeid(T), className);
	}

	CreationFn getCreationFn(const std::string& typeName) const;
	std::optional<std::type_index> getTypeIDFromString(const std::string& typeName) const;
	std::string getStringFromTypeID(const std::type_index& typeID) const;

	BaseComponent* createComponent(const std::string& typeName) const;

	void forEachComponentType(std::function<void(std::type_index, const std::string&)> fn) const;

private:
	std::map<std::string, CreationFn> mComponentCreators;
	std::map<std::string, std::type_index> mStringToTypeID;
	std::map<std::type_index, std::string> mTypeIDToString;
};
