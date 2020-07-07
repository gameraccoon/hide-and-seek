#include "Base/precomp.h"

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/PropertyDescriptors/ComponentAvailabilityPropertyDescriptor.h"

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/TypeFilters/HasComponentPropertyFilter.h"

namespace ImguiPropertyFiltration
{
	ComponentAvailabilityPropertyDescriptor::ComponentAvailabilityPropertyDescriptor(const std::string& componentName, std::type_index typeID)
		: AbstractPropertyDescriptor(componentName)
		, mTypeID(typeID)
	{}

	std::shared_ptr<ComponentAvailabilityPropertyDescriptor> ComponentAvailabilityPropertyDescriptor::Create(const std::string& componentName, std::type_index typeID)
	{
		auto result = std::make_shared<ComponentAvailabilityPropertyDescriptor>(componentName, typeID);
		result->mFilterFactories.push_back(std::make_shared<PropertyFilterFactory<HasComponentPropertyFilter>>(result));
		return result;
	}
} // namespace ImguiPropertyFiltration
