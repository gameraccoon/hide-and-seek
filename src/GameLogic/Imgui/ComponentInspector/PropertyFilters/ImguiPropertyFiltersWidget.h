#pragma once

#include <any>

#include "Base/Types/TemplateAliases.h"

#include "GameData/Debug/SubstringSearcher.h"

class EntityManager;
class WorldCell;
struct ImguiDebugData;

namespace ImguiPropertyFiltration
{
	class AbstractPropertyDescriptor;
	class AbstractPropertyFilter;
	class AbstractPropertyFilterFactory;

	class ImguiPropertyFiltersWidget
	{
	public:
		~ImguiPropertyFiltersWidget();

		void update(ImguiDebugData& debugData);

		void appendFilteredComponentTypes(std::vector<std::type_index>& inOutComponentTypes) const;
		void filterEntities(TupleVector<WorldCell*, Entity>& entities);

	private:
		void init(ImguiDebugData& debugData);

	private:
		static constexpr int mMinimalSearchLen = 2;

		char mFilterQueryBuffer[128] = "";
		SubstringSearcher<std::shared_ptr<AbstractPropertyDescriptor>> mPropertyDescriptors;
		std::vector<std::unique_ptr<AbstractPropertyFilter>> mAppliedFilters;
		std::vector<std::shared_ptr<AbstractPropertyFilterFactory>> mSuggestedFiltersFactories;
		std::vector<std::shared_ptr<AbstractPropertyDescriptor>> mLastMatchedProperties;
		bool mIsInited = false;
	};
} // namespace ImguiPropertyFiltration
