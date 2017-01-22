#include "Actors/Wall.h"


Wall::Wall(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Actor(world, location, rotation)
{
	setType(ActorType::Static);

	setOriginalSize(Vector2D(20, 20));
	setScale(scale);

	Hull geometry;
	geometry.type = Hull::Type::Angular;
	geometry.points.insert(geometry.points.end(), -getOriginalSize()/2);
	geometry.points.insert(geometry.points.end(), (getOriginalSize()/2).mirrorV());
	geometry.points.insert(geometry.points.end(), getOriginalSize()/2);
	geometry.points.insert(geometry.points.end(), (getOriginalSize()/2).mirrorH());
	geometry.generateBorders();
	setGeometry(geometry);
	updateGeometry();

	updateGeometry();

	updateActorId("Wall");
}

Wall::~Wall()
{
}

void Wall::update(float deltatime)
{
	Actor::update(deltatime);
}