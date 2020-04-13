#pragma once

#include <map>
#include <functional>
#include <optional>
#include <typeindex>

#include "ECS/Component.h"

class ComponentFactory
{
public:
	using CreationFn = std::function<BaseComponent*()>;

	template<typename T>
	void registerComponent()
	{
		mComponentCreators[T::GetClassName()] = []{
			return new T();
		};
		StringID className = T::GetClassName();
		mStringToTypeID.emplace(className, typeid(T));
		mTypeIDToString.emplace(typeid(T), className);
	}

	CreationFn getCreationFn(StringID typeName) const;
	std::optional<std::type_index> getTypeIDFromString(StringID typeName) const;
	StringID getStringFromTypeID(const std::type_index& typeID) const;

	BaseComponent* createComponent(StringID typeName) const;

	void forEachComponentType(std::function<void(std::type_index, StringID)> fn) const;

private:
	std::map<StringID, CreationFn> mComponentCreators;
	std::map<StringID, std::type_index> mStringToTypeID;
	std::map<std::type_index, StringID> mTypeIDToString;
};
