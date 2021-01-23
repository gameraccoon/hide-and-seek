#pragma once

#include <unordered_map>
#include <functional>
#include <optional>

#include "ECS/Component.h"

class ComponentFactory
{
public:
	using CreationFn = std::function<BaseComponent*()>;

	ComponentFactory() = default;
	ComponentFactory(ComponentFactory&) = delete;
	ComponentFactory& operator=(ComponentFactory&) = delete;

	template<typename T>
	void registerComponent()
	{
		mComponentCreators[T::GetTypeName()] = []{
			return HS_NEW T();
		};
	}

	CreationFn getCreationFn(StringID typeName) const;

	BaseComponent* createComponent(StringID typeName) const;

	template<typename F>
	void forEachComponentType(F fn) const
	{
		for (auto& creator : mComponentCreators)
		{
			fn(creator.first);
		}
	}

private:
	std::unordered_map<StringID, CreationFn> mComponentCreators;
};
