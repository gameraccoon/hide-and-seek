#include "Modules/ComponentFactory.h"

#include <Assert.h>

BaseComponent*ComponentFactory::NewComponent(const std::string& type, const nlohmann::json& json)
{
	const auto it = mComponentConstructors.find(type);
	if (it != mComponentConstructors.cend())
	{
		BaseComponent* baseComponent = it->second();
		baseComponent->fromJson(json);
		return baseComponent;
	}

	ReportFailure("Unknown component type '%s'", type.c_str());
	return nullptr;
}
