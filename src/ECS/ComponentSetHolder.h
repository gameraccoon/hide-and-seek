#pragma once

#include <tuple>
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
		return mComponents[ComponentType::GetTypeName()] != nullptr;
	}

	template<typename T>
	T* addComponent()
	{
		T* component = HS_NEW T();
		addComponent(component, T::GetTypeName());
		return component;
	}

	template<typename T>
	T* getOrAddComponent()
	{
		auto it = mComponents.find(T::GetTypeName());
		if (it == mComponents.end())
		{
			it = mComponents.emplace(T::GetTypeName(), HS_NEW T()).first;
		}
		return static_cast<T*>(it->second);
	}

	void addComponent(BaseComponent* component, StringID typeID);
	void removeComponent(StringID typeID);

	template<typename... Components>
	std::tuple<Components*...> getComponents()
	{
		return std::make_tuple(getSingleComponent<Components>()...);
	}

	nlohmann::json toJson(const ComponentSerializersHolder& componentSerializers) const;
	void fromJson(const nlohmann::json& json, const ComponentSerializersHolder& componentSerializers);

	[[nodiscard]] bool hasAnyComponents() const;

private:
	template<typename Component>
	Component* getSingleComponent()
	{
		auto it = mComponents.find(Component::GetTypeName());
		if (it != mComponents.end())
		{
			return static_cast<Component*>(it->second);
		}
		else
		{
			return nullptr;
		}
	}

private:
	std::unordered_map<StringID, BaseComponent*> mComponents;
};
