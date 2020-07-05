#pragma once

#include <any>

#include "Base/Types/TemplateAliases.h"

#include "GameData/Debug/SubstringSearcher.h"

class EntityManager;
class WorldCell;

namespace ImguiPropertyFiltration
{
	class AbstractPropertyDescriptor;
	class AbstractPropertyFilter;
	class AbstractPropertyFilterFactory;

	class ImguiPropertyFiltersWidget
	{
	public:
		ImguiPropertyFiltersWidget();
		~ImguiPropertyFiltersWidget();

		void update();

		void appendFilteredComponentTypes(std::vector<std::type_index>& inOutComponentTypes) const;
		void filterEntities(TupleVector<WorldCell*, Entity>& entities);

	private:
		static constexpr int mMinimalSearchLen = 2;

		char mFilterQueryBuffer[128] = "";
		SubstringSearcher<std::shared_ptr<AbstractPropertyDescriptor>> mPropertyDescriptors;
		std::vector<std::unique_ptr<AbstractPropertyFilter>> mAppliedFilters;
		std::vector<std::shared_ptr<AbstractPropertyFilterFactory>> mSuggestedFiltersFactories;
		std::vector<std::shared_ptr<AbstractPropertyDescriptor>> mLastMatchedProperties;
	};
} // namespace ImguiPropertyFiltration
