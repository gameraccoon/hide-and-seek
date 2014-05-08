#include <cfixcc.h>

#include <hge.h>

#include "../src/RayTrace.h"
#include "../src/Actor.h"
#include "../src/World.h"

HGE *TestRayTraceHge;
World *TestRayTraceWorld;

class TestRayTrace : public cfixcc::TestFixture
{
private:
	class TestWall : public Actor
	{
	public:
		TestWall(World *ownerWorld, Vector2D location, Vector2D size) :
			Actor(ownerWorld, ::TestRayTraceHge, location, Rotator(0.f))
		{
			colideBox = BoundingBox(this->location, this->size);
		}
		~TestWall(void) { }
		void update(float deltaTime) { }
		void render(Vector2D shift, Rotator angle) { }
	private:
		void updateCollision() { }
	};
public:

	static void SetUp()
	{
		::TestRayTraceHge = hgeCreate(HGE_VERSION);
		::TestRayTraceWorld = new World();
	}

	static void TearDown()
	{
		::TestRayTraceHge->Release();
		delete ::TestRayTraceWorld;
	}

	void testRayTraceSimpleTrace()
	{
		TestWall wall1(::TestRayTraceWorld, Vector2D(0.f, 0.f), Vector2D(5.f, 10.f));
		
		RayTrace ray1(::TestRayTraceWorld, Vector2D(2.3f, 7.1f), Vector2D(12.7f, -0.3f)); // first point in, second point out

		RayTrace ray2(::TestRayTraceWorld, Vector2D(22.f, 71.2f), Vector2D(124.7f, 90.3f)); // 
		
		CFIXCC_ASSERT_EQUALS(ray1.fastTrace(), true);
		CFIXCC_ASSERT_EQUALS(ray2.fastTrace(), false);
	}
};

CFIXCC_BEGIN_CLASS(TestRayTrace)
	CFIXCC_METHOD(testRayTraceSimpleTrace)
CFIXCC_END_CLASS()

