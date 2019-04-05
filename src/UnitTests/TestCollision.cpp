#include <gtest/gtest.h>

#include <Core/World.h>
#include <Modules/Collide.h>

static CollisionComponent* prepareCollision(World& world, const Vector2D& location, const Hull& hull)
{
	auto entity = world.addEntity();
	auto collision = world.addComponent<CollisionComponent>(entity);
	collision->setGeometry(hull);
	Collide::updateOriginalBoundingBox(collision);
	collision->setBoundingBox(collision->getOriginalBoundingBox() + location);
	return collision;
}

static bool getCollisionResult(World& world, const Vector2D& location1, const Hull& hull1, const Vector2D& location2, const Hull& hull2, Vector2D& outResist)
{
	auto collision1 = prepareCollision(world, location1, hull1);
	auto collision2 = prepareCollision(world, location2, hull2);

	return Collide::doCollide(collision1, location1, collision2, location2, outResist);
}

TEST(Collision, Circular)
{
	World world;

	Hull hull1;
	hull1.type = Hull::Type::Circular;
	hull1.setRadius(10.0f);

	Hull hull2;
	hull2.type = Hull::Type::Circular;
	hull2.setRadius(50.0f);

	Vector2D resist = ZERO_VECTOR;
	EXPECT_TRUE(getCollisionResult(world, Vector2D(20.0f, 10.0f), hull1, Vector2D(-10.0f, 15.0f), hull2, resist));
	EXPECT_FALSE(getCollisionResult(world, Vector2D(50.0f, 10.0f), hull1, Vector2D(-10.0f, 15.0f), hull2, resist));
}

TEST(Collision, Angular)
{
	World world;

	Hull hull1;
	hull1.type = Hull::Type::Circular;
	hull1.setRadius(1.0f);

	Hull hull2;
	hull2.type = Hull::Type::Angular;
	hull2.points.push_back(Vector2D(10.0f, 9.0f));
	hull2.points.push_back(Vector2D(2.0f, 12.0f));
	hull2.points.push_back(Vector2D(3.0f, 5.0f));
	hull2.generateBorders();

	Vector2D resist = ZERO_VECTOR;

	EXPECT_FALSE(getCollisionResult(world, Vector2D(6.0f, 8.0f), hull1, Vector2D(-6.0f, -8.0f), hull2, resist));
	EXPECT_FALSE(getCollisionResult(world, Vector2D(2.0f, 4.0f), hull1, Vector2D(-6.0f, -8.0f), hull2, resist));
	EXPECT_TRUE(getCollisionResult(world, Vector2D(2.0f, 2.0f), hull1, Vector2D(-6.0f, -8.0f), hull2, resist));
	EXPECT_TRUE(getCollisionResult(world, Vector2D(-2.0f, 3.0f), hull1, Vector2D(-6.0f, -8.0f), hull2, resist));
	EXPECT_TRUE(getCollisionResult(world, Vector2D(-2.0f, 1.0f), hull1, Vector2D(-6.0f, -8.0f), hull2, resist));
	EXPECT_FALSE(getCollisionResult(world, Vector2D(4.0f, 3.0f), hull1, Vector2D(-6.0f, -8.0f), hull2, resist));
	EXPECT_TRUE(getCollisionResult(world, Vector2D(-4.5f, 4.2f), hull1, Vector2D(-6.0f, -8.0f), hull2, resist));

	// inverted arguments
	EXPECT_FALSE(getCollisionResult(world, Vector2D(-6.0f, -8.0f), hull2, Vector2D(6.0f, 8.0f), hull1, resist));
	EXPECT_FALSE(getCollisionResult(world, Vector2D(-6.0f, -8.0f), hull2, Vector2D(2.0f, 4.0f), hull1, resist));
	EXPECT_TRUE(getCollisionResult(world, Vector2D(-6.0f, -8.0f), hull2, Vector2D(2.0f, 2.0f), hull1, resist));
	EXPECT_TRUE(getCollisionResult(world, Vector2D(-6.0f, -8.0f), hull2, Vector2D(-2.0f, 3.0f), hull1, resist));
	EXPECT_TRUE(getCollisionResult(world, Vector2D(-6.0f, -8.0f), hull2, Vector2D(-2.0f, 1.0f), hull1, resist));
	EXPECT_FALSE(getCollisionResult(world, Vector2D(-6.0f, -8.0f), hull2, Vector2D(4.0f, 3.0f), hull1, resist));
	EXPECT_TRUE(getCollisionResult(world, Vector2D(-6.0f, -8.0f), hull2, Vector2D(-4.5f, 4.2f), hull1, resist));
}

TEST(Collision, CornerCase1)
{
	World world;

	Hull hull1;
	hull1.type = Hull::Type::Circular;
	hull1.setRadius(32.0f);

	Hull hull2;
	hull2.type = Hull::Type::Angular;
	hull2.points.push_back(Vector2D(60.0, -60.0));
	hull2.points.push_back(Vector2D(60.0, 60.0));
	hull2.points.push_back(Vector2D(-60.0, 60.0));
	hull2.points.push_back(Vector2D(-60.0, -60.0));
	hull2.generateBorders();

	Vector2D resist = ZERO_VECTOR;

	EXPECT_FALSE(getCollisionResult(world, Vector2D(-0.00001f, 1002.0f), hull1, Vector2D(-60.0f, -60.0f), hull2, resist));
}
