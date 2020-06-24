#include "Base/precomp.h"

#include "ECS/ComponentFactory.h"

ComponentFactory::CreationFn ComponentFactory::getCreationFn(StringID className) const
{
	const auto& it = mComponentCreators.find(className);
	if (it != mComponentCreators.cend())
	{
		return it->second;
	}

	ReportFatalError("Unknown component type: '%s'", className);
	return nullptr;
}

std::optional<std::type_index> ComponentFactory::getTypeIDFromClassName(StringID className) const
{
	const auto& it = mClassNameToTypeID.find(className);
	if (it != mClassNameToTypeID.end())
	{
		return it->second;
	}

	ReportFatalError("Unknown component type: '%s'", className);
	return std::nullopt;
}

StringID ComponentFactory::getClassNameFromTypeID(const std::type_index& typeID) const
{
	const auto& it = mTypeIDToClassName.find(typeID);
	if (it != mTypeIDToClassName.end())
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
