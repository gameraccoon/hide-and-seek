#pragma once

#include "ECS/Component.h"

#include "GameData/World.h"

#include "componentreference.h"

namespace Utils
{
	BaseComponent* GetComponent(const ComponentReference& reference, World* world);
	std::vector<BaseComponent*> GetComponents(const ComponentSourceReference& source, World* world);

	template<typename T>
	T* GetComponent(const ComponentSourceReference& source, World* world)
	{
		if (source.isWorld)
		{
			if (source.cellPos.has_value())
			{
				if (source.entity.has_value()) // spatial entity
				{
					if (WorldCell* cell = world->getSpatialData().getCell(*source.cellPos))
					{
						return std::get<0>(cell->getEntityManager().getEntityComponents<T>(*source.entity));
					}
					else
					{
						return nullptr;
					}
				}
				else // cell component
				{
					if (WorldCell* cell = world->getSpatialData().getCell(*source.cellPos))
					{
						return std::get<0>(cell->getCellComponents().getComponents<T>());
					}
					else
					{
						return nullptr;
					}
				}
			}
			else if (source.entity.has_value()) // world entity
			{
				return std::get<0>(world->getEntityManager().getEntityComponents<T>(*source.entity));
			}
			else // world component
			{
				return std::get<0>(world->getWorldComponents().getComponents<T>());
			}
		}
		else // game component
		{
			ReportFatalError("Game Components references are not supported yet");
			return nullptr;
		}
	}
}
