#pragma once

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/AbstractPropertyDescriptor.h"

namespace ImguiPropertyFiltration
{
	class ComponentAvailabilityPropertyDescriptor : public AbstractPropertyDescriptor
	{
	public:
		ComponentAvailabilityPropertyDescriptor(const std::string& componentName, StringID typeID);

		static std::shared_ptr<ComponentAvailabilityPropertyDescriptor> Create(const std::string& componentName, StringID typeID);

		std::any getPropertyValue(EntityManager& /*entityManager*/, Entity /*entity*/) override { return nullptr; }
		StringID getComponentType() const override { return mTypeID; }

	private:
		StringID mTypeID;
	};
}
