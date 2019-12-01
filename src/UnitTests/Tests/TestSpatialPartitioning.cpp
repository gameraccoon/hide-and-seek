#include <gtest/gtest.h>

#include "GameData/Spatial/SpatialWorldData.h"

TEST(SpatialPartitioning, CellPostionTransform)
{
	CellPos cell(20, 2);
	Vector2D pos(SpatialWorldData::CellSize * 1.3f, SpatialWorldData::CellSize * -4.8f);
	SpatialWorldData::TransformCellPos(cell, pos);

	EXPECT_EQ(21, cell.x);
	EXPECT_EQ(-3, cell.y);
	EXPECT_FLOAT_EQ(SpatialWorldData::CellSize * 0.3f, pos.x);
	EXPECT_FLOAT_EQ(SpatialWorldData::CellSize * 0.2f, pos.y);

	pos = Vector2D (SpatialWorldData::CellSize * -8.4f, SpatialWorldData::CellSize * 1.1f);
	SpatialWorldData::TransformCellPos(cell, pos);

	EXPECT_EQ(12, cell.x);
	EXPECT_EQ(-2, cell.y);
	EXPECT_FLOAT_EQ(SpatialWorldData::CellSize * 0.6f, pos.x);
	EXPECT_FLOAT_EQ(SpatialWorldData::CellSize * 0.1f, pos.y);
}
