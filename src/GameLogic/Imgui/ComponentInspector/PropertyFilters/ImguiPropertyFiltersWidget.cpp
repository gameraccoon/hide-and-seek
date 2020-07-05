#include "Base/precomp.h"

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/ImguiPropertyFiltersWidget.h"

#include <string>

#include "imgui.h"

#include "ECS/EntityManager.h"

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/AbstractPropertyFilter.h"
#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/AbstractPropertyDescriptor.h"

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/TypeFilters/FilterRegistration/FilterRegistration.h"
#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/PropertyDescriptorsRegistration.h"

namespace ImguiPropertyFiltration
{
	ImguiPropertyFiltersWidget::ImguiPropertyFiltersWidget()
	{
		mPropertyDescriptors.construct(PropertyDescriptiorsRegistration::GetDescriptions());
	}

	ImguiPropertyFiltersWidget::~ImguiPropertyFiltersWidget() = default;

	void ImguiPropertyFiltersWidget::update()
	{
		std::optional<std::vector<std::unique_ptr<AbstractPropertyFilter>>::const_iterator> filterToRemove;
		for (auto& filter : mAppliedFilters)
		{
			filter->updateImguiWidget();
			ImGui::SameLine();
			if (ImGui::Button("x"))
			{
				filterToRemove = std::find(mAppliedFilters.begin(), mAppliedFilters.end(), filter);
			}
		}
		if (filterToRemove.has_value())
		{
			mAppliedFilters.erase(*filterToRemove);
		}

		if (ImGui::InputText("Property Search", mFilterQueryBuffer, IM_ARRAYSIZE(mFilterQueryBuffer)))
		{
			std::string strId(mFilterQueryBuffer, std::strlen(mFilterQueryBuffer));
			// tolower
			std::transform(strId.begin(), strId.end(), strId.begin(), [](unsigned char c){ return std::tolower(c); });

			if (strId.size() >= mMinimalSearchLen)
			{
				mLastMatchedProperties = mPropertyDescriptors.findAllMatches(strId);
			}
			else
			{
				mLastMatchedProperties.clear();
				mSuggestedFiltersFactories.clear();
			}
		}

		bool isFiltersListShown = false;
		for (const auto& property : mLastMatchedProperties)
		{
			const std::string& propertyName = property->getName();
			ImGui::TextUnformatted(propertyName.c_str());
			ImGui::SameLine();
			if (ImGui::Button(FormatString("Add Filter##%s", propertyName).c_str()))
			{
				mSuggestedFiltersFactories.clear();
				for (const auto& factory : property->getFilterFactories())
				{
					mSuggestedFiltersFactories.push_back(factory);
				}
				isFiltersListShown = true;
				break;
			}
		}
		if (isFiltersListShown)
		{
			mLastMatchedProperties.clear();
			mFilterQueryBuffer[0] = '\0';
		}

		bool isFilterChoosen = false;
		for (const auto& suggestedFilter : mSuggestedFiltersFactories)
		{
			std::string filterName = suggestedFilter->getName().c_str();
			ImGui::TextUnformatted(filterName.c_str());
			ImGui::SameLine();
			if (ImGui::Button(FormatString("Add##%s", filterName).c_str()))
			{
				isFilterChoosen = true;
				mAppliedFilters.push_back(suggestedFilter->createFilter());
				break;
			}
		}
		if (isFilterChoosen)
		{
			mSuggestedFiltersFactories.clear();
		}
	}

	void ImguiPropertyFiltersWidget::appendFilteredComponentTypes(std::vector<std::type_index>& inOutComponentTypes) const
	{
		for (const auto& appliedFilter : mAppliedFilters)
		{
			std::type_index typeID = appliedFilter->getComponentType();
			if (std::find(inOutComponentTypes.begin(), inOutComponentTypes.end(), typeID) == inOutComponentTypes.end())
			{
				inOutComponentTypes.push_back(typeID);
			}
		}
	}

	void ImguiPropertyFiltersWidget::filterEntities(TupleVector<WorldCell*, Entity>& entities)
	{
		for (const auto& filter : mAppliedFilters)
		{
			filter->filterEntities(entities);
		}
	}
} // namespace ImguiPropertyFiltration
