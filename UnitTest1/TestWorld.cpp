#include "stdafx.h"
#include "CppUnitTest.h"


#include <hge.h>

#include "../src/World.cpp"
#include "../src/Actor.cpp"
#include "../src/IActor.cpp"
#include "../src/BoundingBox.cpp"
#include "../src/Hull.cpp"
#include "../src/Border.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	
	HGE *TestWorldHge;
	World *TestedWorld;
	int TestValue;

	TEST_CLASS(TestWorld)
	{
	private:
	class TestActor : public Actor
	{
	public:
		TestActor(World * world) : Actor (world, ZeroVector) { }
		~TestActor() { };

		void Update(float deltaTime)
		{
			TestValue += (int)deltaTime;
		}
		
		void Render(Vector2D shift, Rotator angle) { }
	protected:
		void UpdateCollision() { }
	};
	public:
		TEST_CLASS_INITIALIZE(Init)
		{
			TestWorldHge = hgeCreate(HGE_VERSION);
			TestedWorld = new World(TestWorldHge);
			TestValue = 0;
		}

		TEST_CLASS_CLEANUP(Exit)
		{
			delete TestedWorld;
			TestWorldHge->Release();
		}

		TEST_METHOD(TestWorldAddingStaticActor)
		{
			TestActor actor1(TestedWorld);

			TestValue = 0;

			TestedWorld->Update(1.f);

			Assert::AreEqual(TestValue, 1);
		}

		TEST_METHOD(TestWorldDeletionStaticActor)
		{
			TestValue = 0;

			TestedWorld->Update(1.f);

			Assert::AreEqual(TestValue, 0);
		}

		TEST_METHOD(TestWorldAdditionAndDeletionNonStaticActor)
		{
			TestActor *actor1 = new TestActor(TestedWorld);

			TestValue = 0;

			TestedWorld->Update(1.f);

			Assert::AreEqual(TestValue, 1);

			delete actor1;

			TestValue = 0;

			TestedWorld->Update(1.f);

			Assert::AreEqual(TestValue, 0);

		}
	};
}