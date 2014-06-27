#include "stdafx.h"
#include "CppUnitTest.h"

#include <Engine/Core/World.h>
#include <Engine/Actors/Actor.h>

#include <Engine/Modules/RayTrace.h>

#include "TestWall.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestRaytrace
{
	World *TestWorld;
	IActor *Actor1;

	TEST_CLASS(TestRaytrace)
	{
	public:
		TEST_CLASS_INITIALIZE(Init)
		{
			TestWorld = new World();
			Actor1 = new TestWall(TestWorld, Vector2D(50.f, 30.f), Vector2D(1.f, 1.f), Rotator(0.f)); // size 20-20
		}

		TEST_CLASS_CLEANUP(Exit)
		{
			delete TestWorld;
		}
		
		TEST_METHOD(TestRaytraceFastTrace1)
		{
			Assert::IsTrue(RayTrace::fastTrace(TestWorld, Vector2D(20.f, 20.f), Vector2D(80.f, 60.f))); // out-out

			Assert::IsTrue(RayTrace::fastTrace(TestWorld, Vector2D(30.f, 15.f), Vector2D(55.f, 35.f))); // out-in
			
			Assert::IsFalse(RayTrace::fastTrace(TestWorld, Vector2D(55.f, 35.f), Vector2D(80.f, 60.f))); // in-out

			Assert::IsFalse(RayTrace::fastTrace(TestWorld, Vector2D(80.f, 35.f), Vector2D(80.f, 60.f))); // side

			Assert::IsFalse(RayTrace::fastTrace(TestWorld, Vector2D(20.f, -3.f), Vector2D(80.f, 10.f))); // side
		}
		
		TEST_METHOD(TestRaytraceFastTrace2)
		{
			Assert::IsTrue(RayTrace::fastTrace(TestWorld, Vector2D(35.f, 15.f), Vector2D(65.f, 45.f)));

			Assert::IsTrue(RayTrace::fastTrace(TestWorld, Vector2D(20.f, 60.f), Vector2D(80.f, 0.f)));
		}
		
		TEST_METHOD(TestRaytraceTrace)
		{
			IActor *traceActor = RayTrace::trace(TestWorld, Vector2D(20.f, 20.f), Vector2D(80.f, 60.f));
			
			Assert::IsTrue(traceActor == Actor1);

			IActor *actor = new TestWall(TestWorld, Vector2D(45.f, 25.f), Vector2D(1.f, 1.f), Rotator(0.f)); // size 20-20
			traceActor = RayTrace::trace(TestWorld, Vector2D(20.f, 20.f), Vector2D(80.f, 60.f));

			Assert::IsTrue(traceActor == actor);
			actor->destroy();
		}
	};
}