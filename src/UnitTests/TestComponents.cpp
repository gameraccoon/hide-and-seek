#include <gtest/gtest.h>

#include <Core/World.h>
#include <Components/TransformComponent.h>

TEST(Components, EntityCreationAndRemovement)
{
	World world;
	Entity testEntity1 = world.addEntity();
	Entity testEntity2 = world.addEntity();

	EXPECT_NE(testEntity1, testEntity2);
	EXPECT_NE(testEntity1.getID(), testEntity2.getID());

	world.removeEntity(testEntity2);

	Entity testEntity3 = world.addEntity();

	EXPECT_NE(testEntity1, testEntity3);
	EXPECT_NE(testEntity1.getID(), testEntity3.getID());
}

TEST(Components, ComponentsAttachment)
{
	Vector2D location(Vector2D(1.0f, 0.0f));

	World world;
	Entity testEntity = world.addEntity();
	TransformComponent* transform = world.addComponent<TransformComponent>(testEntity);
	transform->setLocation(location);

	TransformComponent* resultTransform = std::get<0>(world.getEntityComponents<TransformComponent>(testEntity));

	EXPECT_NE(nullptr, transform);
	EXPECT_TRUE(location == resultTransform->getLocation());
}

TEST(Components, RemoveEntityWithComponents)
{
	Vector2D location1(Vector2D(1.0f, 0.0f));
	Vector2D location2(Vector2D(0.0f, 1.0f));
	Vector2D location3(Vector2D(1.0f, 1.0f));

	World world;
	Entity testEntity1 = world.addEntity();
	TransformComponent* transform1 = world.addComponent<TransformComponent>(testEntity1);
	transform1->setLocation(location1);

	Entity testEntity2 = world.addEntity();
	TransformComponent* transform2 = world.addComponent<TransformComponent>(testEntity2);
	transform2->setLocation(location2);

	std::vector<std::tuple<TransformComponent*>> components = world.getComponents<TransformComponent>();
	EXPECT_EQ(static_cast<size_t>(2), components.size());

	bool location1Found = false;
	bool location2Found = false;
	for (auto& component : components)
	{
		Vector2D location = std::get<0>(component)->getLocation();
		if (location == location1 && location1Found == false)
		{
			location1Found = true;
		}
		else if (location == location2 && location2Found == false)
		{
			location2Found = true;
		}
		else
		{
			GTEST_FAIL();
		}
	}
	EXPECT_TRUE(location1Found);
	EXPECT_TRUE(location2Found);

	world.removeEntity(testEntity2);

	Entity testEntity3 = world.addEntity();
	TransformComponent* transform3 = world.addComponent<TransformComponent>(testEntity3);
	transform3->setLocation(location3);

	location1Found = false;
	location2Found = false;
	bool location3Found = false;
	for (auto& component : world.getComponents<TransformComponent>())
	{
		Vector2D location = std::get<0>(component)->getLocation();
		if (location == location1 && location1Found == false)
		{
			location1Found = true;
		}
		else if (location == location2 && location2Found == false)
		{
			location2Found = true;
		}
		else if (location == location3 && location3Found == false)
		{
			location3Found = true;
		}
		else
		{
			GTEST_FAIL();
		}
	}
	EXPECT_TRUE(location1Found);
	EXPECT_FALSE(location2Found);
	EXPECT_TRUE(location3Found);
}
