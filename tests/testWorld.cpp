#include <cfixcc.h>

#include <hge.h>

#include "../src/World.h"
#include "../src/Actor.h"

HGE *TestWorldHge;
World *TestedWorld;
int TestValue;

class testWorld : public cfixcc::TestFixture
{
private:
	class TestActor : public Actor
	{
	public:
		TestActor(World * world) : Actor (world, TestWorldHge, ZeroVector) { }
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

	static void SetUp()
	{
		TestWorldHge = hgeCreate(HGE_VERSION);
		TestedWorld = new World();
		TestValue = 0;
	}

	static void TearDown()
	{
		TestWorldHge->Release();
		delete TestedWorld;
	}

	void testWorldAddingStaticActor()
	{
		TestActor actor1(TestedWorld);

		TestValue = 0;

		TestedWorld->Update(1.f);

		CFIXCC_ASSERT_EQUALS(TestValue, 1);
	}

	void testWorldDeletionStaticActor()
	{
		TestValue = 0;

		TestedWorld->Update(1.f);

		CFIXCC_ASSERT_EQUALS(TestValue, 0);
	}

	void testWorldAdditionAndDeletionNonStaticActor()
	{
		TestActor *actor1 = new TestActor(TestedWorld);

		TestValue = 0;

		TestedWorld->Update(1.f);

		CFIXCC_ASSERT_EQUALS(TestValue, 1);

		delete actor1;

		TestValue = 0;

		TestedWorld->Update(1.f);

		CFIXCC_ASSERT_EQUALS(TestValue, 0);

	}
};

CFIXCC_BEGIN_CLASS(testWorld)
	CFIXCC_METHOD(testWorldAddingStaticActor)
	CFIXCC_METHOD(testWorldDeletionStaticActor)
	CFIXCC_METHOD(testWorldAdditionAndDeletionNonStaticActor)
CFIXCC_END_CLASS()

