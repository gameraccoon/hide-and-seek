#include <gtest/gtest.h>
/*
#include <Core/World.h>
#include <Core/Actor.h>
#include <Modules/ActorFactory.h>
#include <Structures/Hull.h>
#include <Structures/BoundingBox.h>
#include <Structures/Border.h>

namespace
{
	int TestValue;
	int DestructionFlag;
}

class TestWorld : public testing::Test
{
public:
	class TestActor : public Actor
	{
	public:
		TestActor(World * world)
			: Actor(world, ZERO_VECTOR, Rotator(0.f)) { }

		~TestActor()
		{
			DestructionFlag += 1;
		}

		virtual void update(float deltaTime) override final
		{
			TestValue += (int)deltaTime;
		}

		void Render(Vector2D , Rotator ) { }

	protected:
		virtual void hit(IActor *, float , Vector2D ) override final { };
	};

	virtual void SetUp() override
	{
		DestructionFlag = 0;
		TestValue = 0;

		ActorFactory::Factory().registerActor("TestActor", [](World *world, const Vector2D, const Vector2D, const Rotator) {
			return new TestActor(world);
		});
	}

	virtual void TearDown() override
	{
		ActorFactory::Factory().unregisterActor("TestActor");
	}
};

TEST_F(TestWorld, AdditionAndDeletionActor)
{
	std::unique_ptr<World> testWorld(new World());

	IActor *actor1 = ActorFactory::Factory().spawnActor("TestActor", testWorld.get(), Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), Rotator(0.0f));

	TestValue = 0;

	testWorld->update(1.f);

	EXPECT_EQ(1, TestValue);

	actor1->destroy();

	TestValue = 0;

	testWorld->update(1.f);

	EXPECT_EQ(0, TestValue);
}

TEST_F(TestWorld, CleaningUp)
{
	{
		std::unique_ptr<World> testWorld(new World());

		ActorFactory::Factory().spawnActor("TestActor", testWorld.get(), Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), Rotator(0.0f));

		ActorFactory::Factory().spawnActor("TestActor", testWorld.get(), Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), Rotator(0.0f));

		DestructionFlag = 0;
	} // the world and all the actors will be destroyed here

	EXPECT_EQ(2, DestructionFlag);
}
*/
