#include <cfixcc.h>

#include <hge.h>

#include "../src/RayTrace.h"
#include "../src/Actor.h"
#include "../src/World.h"

HGE *TestRayTraceHge;
World *TestRayTraceWorld;

class testRayTrace : public cfixcc::TestFixture
{
private:
	class TestWall : public Actor
	{
	public:
		TestWall(World *ownerWorld, Vector2D location, Vector2D size) : Actor(ownerWorld, TestRayTraceHge, location), Size(size)
		{
			ColideBox = BoundingBox(Location, Size);
		}
		~TestWall(void) { }
		void Update(float deltaTime) { }
		void Render(Vector2D shift, Rotator angle) { }
	private:
		void UpdateCollision() { }
		Vector2D Size;
	};
public:

	static void SetUp()
	{
		TestRayTraceHge = hgeCreate(HGE_VERSION);
		TestRayTraceWorld = new World();
	}

	static void TearDown()
	{
		TestRayTraceHge->Release();
		delete TestRayTraceWorld;
	}

	void testRayTraceSimpleTrace()
	{
		TestWall wall1(TestRayTraceWorld, Vector2D(0.f, 0.f), Vector2D(5.f, 10.f));
		
		RayTrace ray1(TestRayTraceWorld, Vector2D(2.3f, 7.1f), Vector2D(12.7f, -0.3f)); // first point in, second point out

		RayTrace ray2(TestRayTraceWorld, Vector2D(22.f, 71.2f), Vector2D(124.7f, 90.3f)); // 
		
		CFIXCC_ASSERT_EQUALS(ray1.FastTrace(), true);
		CFIXCC_ASSERT_EQUALS(ray2.FastTrace(), false);
	}
};

CFIXCC_BEGIN_CLASS(testRayTrace)
	CFIXCC_METHOD(testRayTraceSimpleTrace)
CFIXCC_END_CLASS()

