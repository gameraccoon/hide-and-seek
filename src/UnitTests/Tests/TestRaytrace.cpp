#include "Base/precomp.h"

#include <gtest/gtest.h>

#include "GameData/World.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Components/CollisionComponent.generated.h"

#include "Utils/Geometry/RayTrace.h"
#include "Utils/Geometry/Collide.h"


Entity FillCollidableObject(World& world)
{
	Vector2D wallPos(50.0f, 30.0f);
	WorldCell& cell = world.getSpatialData().getOrCreateCell(SpatialWorldData::GetCellForPos(wallPos));
	Entity entity = cell.getEntityManager().addEntity();
	TransformComponent* transform = cell.getEntityManager().addComponent<TransformComponent>(entity);
	transform->setLocation(wallPos);
	transform->setRotation(Rotator(0.0f));
	CollisionComponent* collision = cell.getEntityManager().addComponent<CollisionComponent>(entity);
	Hull geometry;
	geometry.type = HullType::Angular;
	geometry.points = {{-10.0f, -10.0f}, {10.0f, -10.0f}, {10.0f, 10.0f}, {-10.0f, 10.0f}};
	geometry.generateBorders();
	collision->setGeometry(geometry);
	Collide::UpdateBoundingBox(collision);
	return entity;
}

TEST(Raytrace, FastTrace1)
{
	World world;
	FillCollidableObject(world);

	EXPECT_TRUE(RayTrace::FastTrace(world, Vector2D(20.f, 20.f), Vector2D(80.f, 60.f))); // out-out

	EXPECT_TRUE(RayTrace::FastTrace(world, Vector2D(30.f, 15.f), Vector2D(55.f, 35.f))); // out-in

	EXPECT_FALSE(RayTrace::FastTrace(world, Vector2D(55.f, 35.f), Vector2D(80.f, 60.f))); // in-out

	EXPECT_FALSE(RayTrace::FastTrace(world, Vector2D(80.f, 35.f), Vector2D(80.f, 60.f))); // side

	EXPECT_FALSE(RayTrace::FastTrace(world, Vector2D(20.f, -3.f), Vector2D(80.f, 10.f))); // side
}

TEST(Raytrace, FastTrace2)
{
	World world;
	FillCollidableObject(world);

	EXPECT_TRUE(RayTrace::FastTrace(world, Vector2D(35.f, 15.f), Vector2D(65.f, 45.f)));

	EXPECT_TRUE(RayTrace::FastTrace(world, Vector2D(20.f, 60.f), Vector2D(80.f, 0.f)));
}

TEST(Raytrace, Trace)
{
	World world;

	Entity entity = FillCollidableObject(world);

	RayTrace::TraceResult traceResult = RayTrace::Trace(world, Vector2D(20.f, 20.f), Vector2D(80.f, 60.f));

	EXPECT_TRUE(traceResult.hasHit);
	EXPECT_EQ(entity, traceResult.hitEntity.entity.getEntity());
}
