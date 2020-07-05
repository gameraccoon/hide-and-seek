#pragma once

namespace ImguiPropertyFiltration
{
	class AbstractPropertyDescriptor;

	namespace PropertyDescriptiorsRegistration
	{
		using DescriptionsRawData = std::vector<std::pair<std::vector<std::string>, std::shared_ptr<AbstractPropertyDescriptor>>>;

		DescriptionsRawData GetDescriptions();
	}
}
