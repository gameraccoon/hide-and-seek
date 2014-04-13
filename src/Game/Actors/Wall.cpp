#include "Wall.h"


// ## This is automatic generated text. Pleace do not change it.
// ## Registration in ActorFactory
#include "../../Engine/Modules/ActorFactory.h"
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
// ## End of automatic generated text


Wall::Wall(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Actor(world, location, rotation)
{
	this->type = AT_Static;

	const int spritelSize = 126;

	this->originalSize = Vector2D(20, 20);
	this->setScale(scale);

	this->geometry.points.insert(this->geometry.points.end(), -this->calculatedSize/2);
	this->geometry.points.insert(this->geometry.points.end(), (this->calculatedSize/2).mirrorV());
	this->geometry.points.insert(this->geometry.points.end(), this->calculatedSize/2);
	this->geometry.points.insert(this->geometry.points.end(), (this->calculatedSize/2).mirrorH());
	this->geometry.generate();

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
	this->colideBox = BoundingBox(this->location - this->calculatedSize/2, this->location + this->calculatedSize/2);
}

void Wall::takeDamage(float damageValue, Vector2D impulse)
{
	// do nothing
}
