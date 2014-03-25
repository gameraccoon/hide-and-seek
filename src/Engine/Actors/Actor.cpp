#include "Actor.h"

Actor::Actor(World *ownerWorld, Vector2D location, Rotator rotation) : location(location),
	colideBox(location, location),
	direction(rotation),
	scale(1.f, 1.f),
	originalSize(1.f, 1.f),
	calculatedSize(1.f, 1.f)
{
	this->ownerWorld = ownerWorld;
	this->ownerWorld->spawnActor(this);
	this->type = AT_Ghost;

	this->classID = "Actor";

	this->bWaitDestruction = false;
}

Actor::~Actor(void)
{
	this->ownerWorld->removeActor(this);
}

void Actor::setLocation(const Vector2D& location)
{
	this->location = Vector2D(location);
}

Vector2D Actor::getLocation()
{
	return this->location;
}

void Actor::setRotation(const Rotator& rotation)
{
	this->direction = Rotator(rotation);
}

Rotator Actor::getRotation()
{
	return this->direction;
}

void Actor::setScale(const Vector2D& scale)
{
	this->scale = Vector2D(scale);
	this->calculatedSize.x = this->originalSize.x * scale.x;
	this->calculatedSize.y = this->originalSize.y * scale.y;
}

Vector2D Actor::getScale()
{
	return this->scale;
}

EActorType Actor::getType()
{
	return this->type;
}

BoundingBox Actor::getBoundingBox()
{
	return this->colideBox;
}

Hull* Actor::getHull()
{
	return &(this->geometry);
}

std::string Actor::getClassID()
{
	return this->classID;
}

void Actor::update(float deltatime)
{
	this->lifetime += deltatime;
}

void Actor::destroy()
{
	if (!this->bWaitDestruction)
	{
		this->bWaitDestruction = true;
	}
}

bool Actor::isWaitDestruction()
{
	return this->bWaitDestruction;
}