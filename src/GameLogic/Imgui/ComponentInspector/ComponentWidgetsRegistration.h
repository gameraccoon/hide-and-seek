#pragma once

#include <map>
#include <memory>

class AbstractComponentImguiWidget;

namespace ComponentWidgetsRegistration
{
	void RegisterInspectWidgets(std::map<StringID, std::unique_ptr<AbstractComponentImguiWidget>>& componentInspectWidgets);
}
