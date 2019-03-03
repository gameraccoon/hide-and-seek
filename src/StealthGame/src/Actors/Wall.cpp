#include "Actors/Wall.h"
#include "Components/RenderComponent.h"
#include "Components/MovementComponent.h"

Wall::Wall(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	: Actor(world)
{
	setType(ActorType::Static);

//	setOriginalSize(Vector2D(20, 20));
	auto movementComponent = makeAndAddComponent<MovementComponent>();
	movementComponent->setLocation(location);
	movementComponent->setRotation(rotation);
	auto renderComponent = makeAndAddComponent<RenderComponent>();
	renderComponent->setScale(scale);

//	Hull geometry;
//	geometry.type = Hull::Type::Angular;
//	geometry.points.insert(geometry.points.end(), -getOriginalSize()/2);
//	geometry.points.insert(geometry.points.end(), (getOriginalSize()/2).mirrorV());
//	geometry.points.insert(geometry.points.end(), getOriginalSize()/2);
//	geometry.points.insert(geometry.points.end(), (getOriginalSize()/2).mirrorH());
//	geometry.generateBorders();
//	setGeometry(geometry);
//	updateGeometry();

//	updateGeometry();

	updateActorId("Wall");
}

Wall::~Wall()
{
}

void Wall::update(float deltatime)
{
	Actor::update(deltatime);
}
