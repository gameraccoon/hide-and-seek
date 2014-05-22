#include "stdafx.h"

#include "TestWall.h"


TestWall::TestWall(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Actor(world, location, rotation)
{
	this->setType(ActorType::Static);

	this->setOriginalSize(Vector2D(20, 20));
	this->setScale(scale);

	Hull geometry;
	geometry.points.insert(geometry.points.end(), -this->getCalculatedSize()/2);
	geometry.points.insert(geometry.points.end(), (this->getCalculatedSize()/2).mirrorV());
	geometry.points.insert(geometry.points.end(), this->getCalculatedSize()/2);
	geometry.points.insert(geometry.points.end(), (this->getCalculatedSize()/2).mirrorH());
	geometry.generate();
	this->setGeometry(geometry);

	this->updateCollision();
}

TestWall::~TestWall(void)
{
}

void TestWall::update(float deltatime)
{
	Actor::update(deltatime);
}

void TestWall::updateCollision()
{
	this->setColideBox(BoundingBox(this->getLocation() - this->getCalculatedSize()/2, this->getLocation() + this->getCalculatedSize()/2));
}