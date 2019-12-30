#pragma once

#include "ECS/Component.h"

#include "GameData/World.h"

#include "componentreference.h"

namespace Utils
{
	template<typename T>
	T* GetComponent<T>(const ComponentReference& reference, World* world)
	{
		if (reference.IsWorld)
		{
			if (reference.CellPos.has_value())
			{
				if (reference.entity.has_value()) // spatial entity
				{
					if (WorldCell* cell = world->getSpatialData().getCell(*reference.cellPos))
					{
						return std::get<0>(cell->getEntityManager().getEntityComponents<T>(*reference.entity));
					}
					else
					{
						return nullptr;
					}
				}
				else // cell component
				{
					if (WorldCell* cell = world->getSpatialData().getCell(*reference.cellPos))
					{
						return std::get<0>(cell->getCellComponents().getComponents<T>());
					}
					else
					{
						return nullptr;
					}
				}
			}
			else if (reference.Entity.has_value()) // world entity
			{
				return std::get<0>(world->getEntityManager().getEntityComponents<T>(*reference.entity));
			}
			else // world component
			{
				return std::get<0>(world->getWorldComponents().getComponents<T>());
			}
		}
		else // game component
		{
			ReportFatalError("Game Components references are not supported yet");
		}
	}
}
