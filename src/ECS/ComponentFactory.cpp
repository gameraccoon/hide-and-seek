#include "ComponentFactory.h"

#include "Debug/Assert.h"

ComponentFactory::CreationFn ComponentFactory::getCreationFn(const std::string& typeName) const
{
	const auto& it = mComponentCreators.find(typeName);
	if (it != mComponentCreators.cend())
	{
		return it->second;
	}

	AssertFatal(false, "Unknown component type: '%s'", typeName.c_str());
	return nullptr;
}

std::optional<std::type_index> ComponentFactory::getTypeIDFromString(const std::string& typeName) const
{
	const auto& it = mStringToTypeID.find(typeName);
	if (it != mStringToTypeID.end())
	{
		return it->second;
	}

	AssertFatal(false, "Unknown component type: '%s'", typeName.c_str());
	return std::nullopt;
}

std::string ComponentFactory::getStringFromTypeID(const std::type_index& typeID) const
{
	const auto& it = mTypeIDToString.find(typeID);
	if (it != mTypeIDToString.end())
	{
		return it->second;
	}

	Assert(false, "Unknown type_index: '%s'", typeID.name());
	return "";
}

BaseComponent* ComponentFactory::createComponent(const std::string& typeName) const
{
	const auto& it = mComponentCreators.find(typeName);
	if (it != mComponentCreators.cend() && it->second)
	{
		return it->second();
	}

	return nullptr;
}

void ComponentFactory::forEachComponentType(std::function<void (std::type_index, const std::string&)> fn) const
{
	if (fn)
	{
		for (auto [typeID, name] : mTypeIDToString)
		{
			fn(typeID, name);
		}
	}
}
