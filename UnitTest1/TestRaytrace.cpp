#include "stdafx.h"
#include "CppUnitTest.h"

#include "../src/Engine/Core/World.h"
#include "../src/Engine/Actors/Actor.h"

#include "../src/Engine/Modules/RayTrace.cpp"
#include "../src/Game/Actors/Wall.cpp"
#include "../src/Engine/Helpers/Log.cpp"

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
			Actor1 = new Wall(TestWorld, Vector2D(50.f, 30.f), Vector2D(1.f, 1.f), Rotator(0.f)); // size 20-20
		}

		TEST_CLASS_CLEANUP(Exit)
		{
			delete TestWorld;
		}
		
		TEST_METHOD(TestRaytraceFastTrace1)
		{
			RayTrace trace1(TestWorld, Vector2D(20.f, 20.f), Vector2D(80.f, 60.f)); // out-out
			Assert::IsTrue(trace1.fastTrace());

			RayTrace trace2(TestWorld, Vector2D(30.f, 15.f), Vector2D(55.f, 35.f)); // out-in
			Assert::IsTrue(trace2.fastTrace());
			
			RayTrace trace3(TestWorld, Vector2D(55.f, 35.f), Vector2D(80.f, 60.f)); // in-out
			Assert::IsFalse(trace3.fastTrace());

			RayTrace trace4(TestWorld, Vector2D(80.f, 35.f), Vector2D(80.f, 60.f)); // side
			Assert::IsFalse(trace4.fastTrace());

			RayTrace trace5(TestWorld, Vector2D(20.f, -3.f), Vector2D(80.f, 10.f)); // side
			Assert::IsFalse(trace5.fastTrace());
		}
		
		TEST_METHOD(TestRaytraceFastTrace2)
		{
			RayTrace trace1(TestWorld, Vector2D(35.f, 15.f), Vector2D(65.f, 45.f));
			Assert::IsTrue(trace1.fastTrace());

			RayTrace trace2(TestWorld, Vector2D(20.f, 60.f), Vector2D(80.f, 0.f));
			Assert::IsTrue(trace2.fastTrace());
		}
		
		TEST_METHOD(TestRaytraceTrace)
		{
			RayTrace trace1(TestWorld, Vector2D(20.f, 20.f), Vector2D(80.f, 60.f));
			IActor *traceActor = trace1.trace();
			
			Assert::IsTrue(traceActor == Actor1);

			IActor *actor = new Wall(TestWorld, Vector2D(45.f, 25.f), Vector2D(1.f, 1.f), Rotator(0.f)); // size 20-20
			traceActor = trace1.trace();

			Assert::IsTrue(traceActor == actor);
			actor->destroy();
		}
	};
}