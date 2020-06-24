#pragma once

#include <unordered_map>
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
		mClassNameToTypeID.emplace(className, typeid(T));
		mTypeIDToClassName.emplace(typeid(T), className);
	}

	CreationFn getCreationFn(StringID typeName) const;
	std::optional<std::type_index> getTypeIDFromClassName(StringID className) const;
	StringID getClassNameFromTypeID(const std::type_index& typeID) const;

	BaseComponent* createComponent(StringID typeName) const;

	template<typename F>
	void forEachComponentType(F fn) const
	{
		if (fn)
		{
			for (auto [typeID, name] : mTypeIDToClassName)
			{
				fn(typeID, name);
			}
		}
	}

private:
	std::unordered_map<StringID, CreationFn> mComponentCreators;
	std::unordered_map<StringID, std::type_index> mClassNameToTypeID;
	std::unordered_map<std::type_index, StringID> mTypeIDToClassName;
};
