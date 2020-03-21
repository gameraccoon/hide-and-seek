#include "Base/precomp.h"

#include <gtest/gtest.h>
/*
#include <Core/World.h>
#include <Core/Actor.h>

#include <Modules/RayTrace.h>
#include <Modules/ActorFactory.h>

#include "TestWall.h"


class TestRaytrace : public ::testing::Test
{
public:
	void SetUp() override
	{
		TestWorld.reset(new World());
		ActorFactory::Factory().registerActor("TestWall", [](World *world, const Vector2D location, const Vector2D size, const Rotator rotation) {
			return new TestWall(world, location, size, rotation);
		});
		Actor1 = ActorFactory::Factory().spawnActor("TestWall", TestWorld.get(), Vector2D(50.f, 30.f), Vector2D(1.f, 1.f), Rotator(0.f));
	}

	void TearDown() override
	{
		ActorFactory::Factory().unregisterActor("TestWall");
	}

	std::unique_ptr<World> TestWorld;
	IActor* Actor1; // exists only while the world exists
};

TEST_F(TestRaytrace, FastTrace1)
{
	EXPECT_TRUE(RayTrace::fastTrace(TestWorld.get(), Vector2D(20.f, 20.f), Vector2D(80.f, 60.f))); // out-out

	EXPECT_TRUE(RayTrace::fastTrace(TestWorld.get(), Vector2D(30.f, 15.f), Vector2D(55.f, 35.f))); // out-in
			
	EXPECT_FALSE(RayTrace::fastTrace(TestWorld.get(), Vector2D(55.f, 35.f), Vector2D(80.f, 60.f))); // in-out

	EXPECT_FALSE(RayTrace::fastTrace(TestWorld.get(), Vector2D(80.f, 35.f), Vector2D(80.f, 60.f))); // side

	EXPECT_FALSE(RayTrace::fastTrace(TestWorld.get(), Vector2D(20.f, -3.f), Vector2D(80.f, 10.f))); // side
}

TEST_F(TestRaytrace, FastTrace2)
{
	EXPECT_TRUE(RayTrace::fastTrace(TestWorld.get(), Vector2D(35.f, 15.f), Vector2D(65.f, 45.f)));

	EXPECT_TRUE(RayTrace::fastTrace(TestWorld.get(), Vector2D(20.f, 60.f), Vector2D(80.f, 0.f)));
}

TEST_F(TestRaytrace, Trace)
{
	IActor *traceActor = RayTrace::trace(TestWorld.get(), Vector2D(20.f, 20.f), Vector2D(80.f, 60.f));
			
	EXPECT_TRUE(traceActor == Actor1);

	IActor *actor = ActorFactory::Factory().spawnActor("TestWall", TestWorld.get(), Vector2D(45.f, 25.f), Vector2D(1.f, 1.f), Rotator(0.f));
	traceActor = RayTrace::trace(TestWorld.get(), Vector2D(20.f, 20.f), Vector2D(80.f, 60.f));

	EXPECT_TRUE(traceActor == actor);
	actor->destroy();
}
*/
