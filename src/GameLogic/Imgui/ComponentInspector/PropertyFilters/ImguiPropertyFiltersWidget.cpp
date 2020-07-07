#include "Base/precomp.h"

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/ImguiPropertyFiltersWidget.h"

#include <string>

#include "imgui.h"

#include "ECS/EntityManager.h"

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/AbstractPropertyFilter.h"
#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/AbstractPropertyDescriptor.h"

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/TypeFilters/FilterRegistration/FilterRegistration.h"
#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/PropertyDescriptorsRegistration.h"

#include "GameLogic/Imgui/ComponentInspector/PropertyFilters/PropertyDescriptors/ComponentAvailabilityPropertyDescriptor.h"

#include "GameLogic/Imgui/ImguiDebugData.h"

namespace ImguiPropertyFiltration
{
	void ImguiPropertyFiltersWidget::init(ImguiDebugData& debugData)
	{
		auto propertyDescriptions = PropertyDescriptiorsRegistration::GetDescriptions();
		debugData.componentFactory.forEachComponentType([&propertyDescriptions](std::type_index typeID, StringID className)
		{
			std::string componentName = ID_TO_STR(className);
			std::string lowerComponentName = componentName;
			std::transform(lowerComponentName.begin(), lowerComponentName.end(), lowerComponentName.begin(), [](unsigned char c){ return std::tolower(c); });
			propertyDescriptions.emplace_back(std::vector<std::string>{lowerComponentName}, ComponentAvailabilityPropertyDescriptor::Create(componentName, typeID));
		});
		mPropertyDescriptors.construct(std::move(propertyDescriptions));
	}

	ImguiPropertyFiltersWidget::~ImguiPropertyFiltersWidget() = default;

	void ImguiPropertyFiltersWidget::update(ImguiDebugData& debugData)
	{
		if (!mIsInited)
		{
			init(debugData);
			mIsInited = true;
		}

		std::optional<std::vector<std::unique_ptr<AbstractPropertyFilter>>::const_iterator> filterToRemove;
		for (auto& filter : mAppliedFilters)
		{
			filter->updateImguiWidget();
			ImGui::SameLine();
			if (ImGui::Button(FormatString("x##%s", filter->getName()).c_str()))
			{
				filterToRemove = std::find(mAppliedFilters.begin(), mAppliedFilters.end(), filter);
			}
		}
		if (filterToRemove.has_value())
		{
			mAppliedFilters.erase(*filterToRemove);
		}

		if (ImGui::InputTextWithHint("##FilterSearch", "Search Query", mFilterQueryBuffer, IM_ARRAYSIZE(mFilterQueryBuffer)))
		{
			{
				Entity::EntityID id = 0;
				std::string_view strId(mFilterQueryBuffer, IM_ARRAYSIZE(mFilterQueryBuffer));
				std::stringstream ss;
				if (strId[0] == '0' && strId[1] == 'x') {
					ss << std::hex;
				}
				ss << strId;
				ss >> id;
				Entity entity(id);

				SpatialEntityManager allManagers = debugData.worldHolder.getWorld().getSpatialData().getAllCellManagers();
				WorldCell* cell = allManagers.findEntityCell(entity);
				if (cell != nullptr)
				{
					mExplicitlySetEntity = std::make_tuple(cell, entity);
					return;
				}
				else
				{
					mExplicitlySetEntity = std::nullopt;
				}
			}

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

	std::vector<std::type_index> ImguiPropertyFiltersWidget::getFilteredComponentTypes() const
	{
		std::vector<std::type_index> filteredComponents;
		filteredComponents.reserve(mAppliedFilters.size());
		// construct vector of unique elements
		for (const auto& appliedFilter : mAppliedFilters)
		{
			std::type_index typeID = appliedFilter->getComponentType();
			auto lowerIt = std::lower_bound(filteredComponents.begin(), filteredComponents.end(), typeID);
			if (lowerIt == filteredComponents.end() || *lowerIt != typeID)
			{
				filteredComponents.insert(lowerIt, typeID);
			}
		}
		return filteredComponents;
	}

	TupleVector<WorldCell*, Entity> ImguiPropertyFiltersWidget::getFilteredEntities(ImguiDebugData& debugData)
	{
		TupleVector<WorldCell*, Entity> result;
		result.reserve(50);

		std::vector<std::type_index> filteredComponentTypes = getFilteredComponentTypes();

		if (!filteredComponentTypes.empty())
		{
			SpatialEntityManager allManagers = debugData.worldHolder.getWorld().getSpatialData().getAllCellManagers();
			allManagers.getSpatialEntitiesHavingComponents(filteredComponentTypes, result);

			for (const auto& filter : mAppliedFilters)
			{
				filter->filterEntities(result);
			}
		}
		else if (mExplicitlySetEntity.has_value())
		{
			result.push_back(*mExplicitlySetEntity);
		}

		return result;
	}
} // namespace ImguiPropertyFiltration
