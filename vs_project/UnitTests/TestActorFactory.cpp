#include "stdafx.h"
#include "CppUnitTest.h"

#include <Engine/Actors/Actor.h>

#include <Engine/Modules/ActorFactory.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestActorFactory
{
	int TestValue;
	World *TestWorld;

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
		
		void Render(Vector2D shift, Rotator angle) { }
	protected:
		virtual void hit(IActor *instigator, float damageValue, Vector2D impulse) override final { };
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
		
		void Render(Vector2D shift, Rotator angle) { }
	protected:
		virtual void hit(IActor *instigator, float damageValue, Vector2D impulse) override final { };
	};

	// specific factory
	IActor* CreateTestActor1(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new TestFactoryActor1(world);
	}

	// specific factory
	IActor* CreateTestActor2(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new TestFactoryActor2(world);
	}
	
	TEST_CLASS(TestActorFactory)
	{
	public:
		TEST_CLASS_INITIALIZE(Init)
		{
			TestWorld = new World();
		}

		TEST_CLASS_CLEANUP(Exit)
		{
			delete TestWorld;
		}

		TEST_METHOD(TestActorFactoryRegister)
		{
			bool registered = ActorFactory::Factory().registerActor("TestActor", CreateTestActor1);

			Assert::IsTrue(registered);

			IActor *actor = ActorFactory::Factory().placeActor("TestActor", TestWorld,
				ZERO_VECTOR, Vector2D(1.f, 1.f), Rotator(0.0));

			TestValue = 0;

			TestWorld->update(1);

			actor->destroy();

			Assert::AreEqual(1, TestValue);

			ActorFactory::Factory().unregisterActor("TestActor");
		}

		TEST_METHOD(TestActorFactoryUnregister)
		{
			bool registered = ActorFactory::Factory().registerActor("TestActor", CreateTestActor1);

			Assert::IsTrue(registered);

			bool unregistered = ActorFactory::Factory().unregisterActor("TestActor");

			Assert::IsTrue(unregistered);

			int final = 0;

			try
			{
				IActor *actor = ActorFactory::Factory().placeActor("TestActor", TestWorld,
					ZERO_VECTOR, Vector2D(1.f, 1.f), Rotator(0.0));
			}
			catch (std::runtime_error)
			{
				final = 1;
			}
			
			Assert::AreEqual(1, final);
		}

		TEST_METHOD(TestActorFactoryDoubleRegistration)
		{
			bool registered = ActorFactory::Factory().registerActor("TestActor", CreateTestActor1); // "TestActor" -> CreateTestActor1

			Assert::IsTrue(registered);

			registered = ActorFactory::Factory().registerActor("TestActor", CreateTestActor1); // "TestActor" -> CreateTestActor1

			Assert::IsFalse(registered);

			IActor *actor = ActorFactory::Factory().placeActor("TestActor", TestWorld,
				ZERO_VECTOR, Vector2D(1.f, 1.f), Rotator(0.0));
			
			TestValue = 0;

			TestWorld->update(1);

			actor->destroy();

			Assert::AreEqual(1, TestValue);

			bool unregistered = ActorFactory::Factory().unregisterActor("TestActor"); // "TestActor" -> none

			Assert::IsTrue(unregistered);

			int final = 0;

			try
			{
				IActor *actor = ActorFactory::Factory().placeActor("TestActor", TestWorld,
					ZERO_VECTOR, Vector2D(1.f, 1.f), Rotator(0.0));
			}
			catch (std::runtime_error)
			{
				final = 1;
			}
			
			Assert::AreEqual(1, final);

			unregistered = ActorFactory::Factory().unregisterActor("TestActor");

			Assert::IsFalse(unregistered);
		}
	};
}