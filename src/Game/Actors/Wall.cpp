#include "Wall.h"


#include <Modules/ActorFactory.h>
// unnamed namespase to hide from another places
namespace
{
	// specific factory
	IActor* CreateWall(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new Wall(world, location, scale, rotation);
	}

	const std::string CLASS_ID = "Wall";

	// register specific factory in actor factory
	const bool registered = ActorFactory::Factory().registerActor(CLASS_ID, CreateWall);
}


Wall::Wall(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Actor(world, location, rotation)
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

	this->updateActorId(CLASS_ID);
}

Wall::~Wall(void)
{
}

void Wall::update(float deltatime)
{
	Actor::update(deltatime);
}

void Wall::updateCollision()
{
	this->setColideBox(BoundingBox(this->getLocation() - this->getCalculatedSize()/2, this->getLocation() + this->getCalculatedSize()/2));
}