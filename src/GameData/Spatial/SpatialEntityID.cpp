#include "GameData/Spatial/SpatialEntityID.h"

SpatialEntityID::SpatialEntityID(Entity entity, CellPos cellPos)
	: entity(entity)
	, cell(cellPos)
{
}
