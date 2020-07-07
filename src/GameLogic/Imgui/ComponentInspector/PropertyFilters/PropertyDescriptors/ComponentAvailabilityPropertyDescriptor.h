#pragma once

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/AbstractPropertyDescriptor.h"

namespace ImguiPropertyFiltration
{
	class ComponentAvailabilityPropertyDescriptor : public AbstractPropertyDescriptor
	{
	public:
		ComponentAvailabilityPropertyDescriptor(const std::string& componentName, std::type_index typeID);

		static std::shared_ptr<ComponentAvailabilityPropertyDescriptor> Create(const std::string& componentName, std::type_index typeID);

		std::any getPropertyValue(EntityManager& /*entityManager*/, Entity /*entity*/) override { return nullptr; }
		std::type_index getComponentType() const override { return mTypeID; }

	private:
		std::type_index mTypeID;
	};
}
