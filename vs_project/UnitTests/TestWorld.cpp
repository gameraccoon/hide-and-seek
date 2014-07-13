#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../src/Engine/Core/IActor.h"

#include <Engine/Core/World.h>
#include <Engine/Actors/Actor.h>
#include <Engine/Structures/Hull.h>
#include <Engine/Structures/BoundingBox.h>
#include <Engine/Structures/Border.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestWorld
{
	int TestValue;
	int DestructionFlag;

	TEST_CLASS(TestWorld)
	{
	private:
		class TestActor : public Actor
		{
		public:
			TestActor(World * world) : Actor(world, ZERO_VECTOR, Rotator(0.f)) { }
			
			~TestActor()
			{
				DestructionFlag += 1;
			}

			virtual void update(float deltaTime) override final
			{
				TestValue += (int)deltaTime;
			}
		
			void Render(Vector2D shift, Rotator angle) { }
		protected:
			virtual void hit(IActor *instigator, float damageValue, Vector2D impulse) override final { };
		};
	public:
		TEST_CLASS_INITIALIZE(Init)
		{
			DestructionFlag = 0;
			TestValue = 0;
		}

		TEST_METHOD(TestWorldAdditionAndDeletionActor)
		{
			World *testWorld = new World();

			IActor *actor1 = new TestActor(testWorld);

			TestValue = 0;

			testWorld->update(1.f);

			Assert::AreEqual(1, TestValue);

			actor1->destroy();

			TestValue = 0;

			testWorld->update(1.f);

			Assert::AreEqual(0, TestValue);

			delete testWorld;
		}

		TEST_METHOD(TestWorldCleaningUp)
		{
			World *testWorld = new World();

			IActor *actor1 = new TestActor(testWorld);

			IActor *actor2 = new TestActor(testWorld);

			DestructionFlag = 0;

			delete testWorld; // delete actor1; delete actor2;

			Assert::AreEqual(2, DestructionFlag);
		}
	};
}