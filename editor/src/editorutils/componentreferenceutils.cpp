#include "componentreferenceutils.h"

#include "GameData/World.h"

namespace Utils
{
	BaseComponent* GetComponent(const ComponentReference& reference, World* world)
	{
		std::vector<BaseComponent*> components = GetComponents(reference.source, world);
		for (BaseComponent* component : components)
		{
			if (component->getComponentTypeName() == reference.componentTypeName)
			{
				return component;
			}
		}
		return nullptr;
	}

	std::vector<BaseComponent*> GetComponents(const ComponentSourceReference& source, World* world)
	{
		if (world)
		{
			if (source.isWorld)
			{
				if (source.entity.has_value())
				{
					if (source.cellPos.has_value())
					{
						// spatial entities
						if (WorldCell* cell = world->getSpatialData().getCell(*source.cellPos))
						{
							return cell->getEntityManager().getAllEntityComponents(*source.entity);
						}
					}
					else
					{
						// world entities
						return world->getEntityManager().getAllEntityComponents(*source.entity);
					}
				}
				else
				{
					// cell components
					if (source.cellPos.has_value())
					{
						if (WorldCell* cell = world->getSpatialData().getCell(*source.cellPos))
						{
							return cell->getCellComponents().getAllComponents();
						}
					}
					else
					{
						// world components
						return world->getWorldComponents().getAllComponents();
					}
				}
			}
			else
			{
				// game components
				ReportError("Game components (or any others not belonging to a world) are not supported yet");
			}
		}
		return std::vector<BaseComponent*>();
	}
}
