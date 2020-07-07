#pragma once

#include <tuple>
#include <typeindex>
#include <map>
#include <vector>

#include "Component.h"
#include "Delegates.h"

struct ComponentSerializersHolder;

/**
 * Use this class to store components specific for some non-entity object (e.g. for a World)
 */
class ComponentSetHolder
{
public:
	ComponentSetHolder() = default;
	~ComponentSetHolder();

	ComponentSetHolder(const ComponentSetHolder&) = delete;
	ComponentSetHolder& operator=(const ComponentSetHolder&) = delete;
	ComponentSetHolder(ComponentSetHolder&&) = default;
	ComponentSetHolder& operator=(ComponentSetHolder&&) = default;

	std::vector<BaseComponent*> getAllComponents();

	template<typename ComponentType>
	bool doesComponentExists()
	{
		return mComponents[typeid(ComponentType)] != nullptr;
	}

	template<typename T>
	T* addComponent()
	{
		T* component = new T();
		addComponent(component, typeid(T));
		return component;
	}

	void addComponent(BaseComponent* component, std::type_index typeID);
	void removeComponent(std::type_index typeID);

	template<typename... Components>
	std::tuple<Components*...> getComponents()
	{
		return std::make_tuple(static_cast<Components*>(mComponents[typeid(Components)])...);
	}

	nlohmann::json toJson(const ComponentSerializersHolder& componentSerializers) const;
	void fromJson(const nlohmann::json& json, const ComponentSerializersHolder& componentSerializers);

private:
	std::map<std::type_index, BaseComponent*> mComponents;
};
