#include "stdafx.h"

#include "TestWall.h"


TestWall::TestWall(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Actor(world, location, rotation)
{
	this->setType(ActorType::Static);

	this->setOriginalSize(Vector2D(20, 20));
	this->setScale(scale);

	Hull geometry;
	geometry.type = Hull::Type::Angular;
	geometry.points.insert(geometry.points.end(), -this->getCalculatedSize()/2);
	geometry.points.insert(geometry.points.end(), (this->getCalculatedSize()/2).mirrorV());
	geometry.points.insert(geometry.points.end(), this->getCalculatedSize()/2);
	geometry.points.insert(geometry.points.end(), (this->getCalculatedSize()/2).mirrorH());
	geometry.generateBorders();
	this->setGeometry(geometry);

	this->updateGeometry();
}

TestWall::~TestWall(void)
{
}

void TestWall::update(float deltatime)
{
	Actor::update(deltatime);
}