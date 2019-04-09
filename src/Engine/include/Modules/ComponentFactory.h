#pragma once

#include <map>
#include <string>
#include <functional>

#include <nlohmann/json_fwd.hpp>

#include <Core/Component.h>

class ComponentFactory
{
	typedef std::function<BaseComponent*()> ConstructionFn;

	template<typename T>
	void RegisterComponentConstructor(const std::string& type)
	{
		mComponentConstructors[type] = []() {
			return new T();
		};
	}

	BaseComponent* NewComponent(const std::string& type, const nlohmann::json& json);

private:
	std::map<std::string, ConstructionFn> mComponentConstructors;
};
