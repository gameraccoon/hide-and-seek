#pragma once

#include <memory>
#include <any>

#include "ECS/Entity.h"

class EntityManager;

namespace ImguiPropertyFiltration
{
	class AbstractPropertyFilterFactory;

	class AbstractPropertyDescriptor
	{
	public:
		AbstractPropertyDescriptor(const std::string& name) : mName(name) {}
		virtual ~AbstractPropertyDescriptor() = default;

		virtual std::any getPropertyValue(EntityManager& entityManager, Entity entity) = 0;
		virtual StringID getComponentType() const = 0;

		const std::string& getName() const { return mName; }
		const std::vector<std::shared_ptr<AbstractPropertyFilterFactory>>& getFilterFactories() const { return mFilterFactories; }
		bool hasFactories() const { return !mFilterFactories.empty(); }

	protected:
		std::vector<std::shared_ptr<AbstractPropertyFilterFactory>> mFilterFactories;

	private:
		std::string mName;
	};
} // namespace ImguiPropertyFiltration
