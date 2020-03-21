#include "Base/precomp.h"

#include "ECS/ComponentFactory.h"

#include "Base/Debug/Assert.h"

ComponentFactory::CreationFn ComponentFactory::getCreationFn(StringID typeName) const
{
	const auto& it = mComponentCreators.find(typeName);
	if (it != mComponentCreators.cend())
	{
		return it->second;
	}

	ReportFatalError("Unknown component type: '%s'", typeName);
	return nullptr;
}

std::optional<std::type_index> ComponentFactory::getTypeIDFromString(StringID typeName) const
{
	const auto& it = mStringToTypeID.find(typeName);
	if (it != mStringToTypeID.end())
	{
		return it->second;
	}

	ReportFatalError("Unknown component type: '%s'", typeName);
	return std::nullopt;
}

StringID ComponentFactory::getStringFromTypeID(const std::type_index& typeID) const
{
	const auto& it = mTypeIDToString.find(typeID);
	if (it != mTypeIDToString.end())
	{
		return it->second;
	}

	ReportError("Unknown type_index: '%s'", typeID.name());
	return StringID();
}

BaseComponent* ComponentFactory::createComponent(StringID typeName) const
{
	const auto& it = mComponentCreators.find(typeName);
	if (it != mComponentCreators.cend() && it->second)
	{
		return it->second();
	}

	return nullptr;
}

void ComponentFactory::forEachComponentType(std::function<void (std::type_index, StringID)> fn) const
{
	if (fn)
	{
		for (auto [typeID, name] : mTypeIDToString)
		{
			fn(typeID, name);
		}
	}
}
