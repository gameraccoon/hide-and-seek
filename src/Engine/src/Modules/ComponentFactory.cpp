#include "Modules/ComponentFactory.h"

#include <Assert.h>

ComponentFactory::DeserializationFn ComponentFactory::getDeserializator(const std::string& type) const
{
	const auto& it = mComponentDeserializators.find(type);
	if (it != mComponentDeserializators.cend())
	{
		return it->second;
	}

	return nullptr;
}

std::optional<std::type_index> ComponentFactory::getTypeIDFromString(const std::string& type) const
{
	const auto& it = mStringToTypeID.find(type);
	if (it != mStringToTypeID.end())
	{
		return it->second;
	}

	return std::nullopt;
}

std::string ComponentFactory::getStringFromTypeID(const std::type_index& typeID) const
{
	const auto& it = mTypeIDToString.find(typeID);
	if (it != mTypeIDToString.end())
	{
		return it->second;
	}

	ReportFailure("Unknown type_index: '%s'", typeID.name());
	return "";
}
