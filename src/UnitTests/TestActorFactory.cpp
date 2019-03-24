#include <gtest/gtest.h>
/*
#include <memory>

#include <Core/Actor.h>
#include <Modules/ActorFactory.h>

namespace
{
	int TestValue;

	// new actor 1
	class TestFactoryActor1 : public Actor
	{
	public:
		TestFactoryActor1(World * world) : Actor(world, ZERO_VECTOR, Rotator(0.f)) { }

		~TestFactoryActor1() { }

		virtual void update(float deltaTime) override final
		{
			TestValue += (int)deltaTime;
		}

		void Render(Vector2D , Rotator ) { }
	protected:
		virtual void hit(IActor *, float , Vector2D ) override final { };
	};

	// new actor 2
	class TestFactoryActor2 : public Actor
	{
	public:
		TestFactoryActor2(World * world) : Actor(world, ZERO_VECTOR, Rotator(0.f)) { }

		~TestFactoryActor2() { }

		virtual void update(float deltaTime) override final
		{
			TestValue += (int)deltaTime * 2;
		}

		void Render(Vector2D , Rotator ) { }
	protected:
		virtual void hit(IActor *, float , Vector2D ) override final { };
	};

	// specific factory
	IActor* CreateTestActor1(World *world, const Vector2D , const Vector2D , const Rotator )
	{
		return new TestFactoryActor1(world);
	}

	// specific factory
	IActor* CreateTestActor2(World *world, const Vector2D , const Vector2D , const Rotator )
	{
		return new TestFactoryActor2(world);
	}
}

class TestActorFactory : public ::testing::Test
{
public:
	virtual void SetUp() override
	{
		TestWorld.reset(new World());
	}

	virtual void TearDown() override
	{
	}

	std::unique_ptr<World> TestWorld;
};


TEST_F(TestActorFactory, Register)
{
	bool registered = ActorFactory::Factory().registerActor("TestActor", CreateTestActor1);

	EXPECT_TRUE(registered);

	IActor *actor = ActorFactory::Factory().spawnActor("TestActor", TestWorld.get(),
		ZERO_VECTOR, Vector2D(1.f, 1.f), Rotator(0.0));

	TestValue = 0;

	TestWorld->update(1);

	actor->destroy();

	EXPECT_EQ(1, TestValue);

	ActorFactory::Factory().unregisterActor("TestActor");
}

TEST_F(TestActorFactory, Unregister)
{
	bool registered = ActorFactory::Factory().registerActor("TestActor", CreateTestActor1);

	EXPECT_TRUE(registered);

	bool unregistered = ActorFactory::Factory().unregisterActor("TestActor");

	EXPECT_TRUE(unregistered);

	EXPECT_THROW(
		ActorFactory::Factory().spawnActor("TestActor", TestWorld.get(),
			ZERO_VECTOR, Vector2D(1.f, 1.f), Rotator(0.0));
		, std::runtime_error
	);
}

TEST_F(TestActorFactory, DoubleRegistration)
{
	bool registered = ActorFactory::Factory().registerActor("TestActor", CreateTestActor1); // "TestActor" -> CreateTestActor1

	EXPECT_TRUE(registered);

	registered = ActorFactory::Factory().registerActor("TestActor", CreateTestActor1); // "TestActor" -> CreateTestActor1

	EXPECT_FALSE(registered);

	IActor *actor = ActorFactory::Factory().spawnActor("TestActor", TestWorld.get(),
		ZERO_VECTOR, Vector2D(1.f, 1.f), Rotator(0.0));

	TestValue = 0;

	TestWorld->update(1);

	actor->destroy();

	EXPECT_EQ(1, TestValue);

	bool unregistered = ActorFactory::Factory().unregisterActor("TestActor"); // "TestActor" -> none

	EXPECT_TRUE(unregistered);

	EXPECT_THROW(
		ActorFactory::Factory().spawnActor("TestActor", TestWorld.get(),
			ZERO_VECTOR, Vector2D(1.f, 1.f), Rotator(0.0));
		, std::runtime_error
	);

	unregistered = ActorFactory::Factory().unregisterActor("TestActor");

	EXPECT_FALSE(unregistered);
}
*/
