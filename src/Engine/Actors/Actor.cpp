#include "Actor.h"

#include <sstream>

namespace
{
	int ClassIndex = 0;
}

Actor::Actor(World *world, Vector2D location, Rotator rotation) : location(location),
    scale(1.f, 1.f),
    originalSize(1.f, 1.f),
    calculatedSize(1.f, 1.f),
    direction(rotation),
    colideBox(location, location)
{
	this->ownerWorld = world;
	this->ownerWorld->spawnActor(this);
	this->type = ActorType::Ghost;

	this->updateActorId("Actor");

	this->bWaitDestruction = false;

	this->lifetime = 0;
}

Actor::~Actor(void)
{
	this->ownerWorld->removeActor(this);
}

void Actor::setLocation(const Vector2D& location)
{
	this->location = Vector2D(location);
	this->updateCollision();
	this->onUpdateLocation();
}

Vector2D Actor::getLocation() const
{
	return this->location;
}

void Actor::setRotation(const Rotator& rotation)
{
	this->direction = Rotator(rotation);
	this->updateCollision();
	this->onUpdateRotation();
	this->updateGeometry();
}

Rotator Actor::getRotation() const
{
	return this->direction;
}

void Actor::setScale(const Vector2D& scale)
{
	this->scale = Vector2D(scale);
	this->calculatedSize.x = this->originalSize.x * scale.x;
	this->calculatedSize.y = this->originalSize.y * scale.y;
	this->onUpdateSize();
	this->updateGeometry();
}

Vector2D Actor::getScale() const
{
	return this->scale;
}

ActorType Actor::getType() const
{
	return this->type;
}

BoundingBox Actor::getBoundingBox() const
{
	return this->colideBox;
}

const Hull* Actor::getGeometry() const
{
	return &(this->geometry);
}

std::string Actor::getClassID() const
{
	return this->classId;
}

std::string Actor::getActorId() const
{
	return this->actorId;
}

void Actor::updateActorId(std::string classId)
{
	this->classId = classId;
	std::ostringstream s;
	s << ::ClassIndex++;
	this->actorId = classId + s.str();
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

bool Actor::isWaitDestruction() const
{
	return this->bWaitDestruction;
}

void Actor::hit(IActor *instigator, float damageValue, Vector2D impulse)
{
}

World* Actor::getOwnerWorld() const
{
	return this->ownerWorld;
}

float Actor::getLifetime() const
{
	return this->lifetime;
}

Vector2D Actor::getCalculatedSize() const
{
	return this->calculatedSize;
}

void Actor::setType(ActorType newType)
{
	this->type = newType;
}

void Actor::setOriginalSize(Vector2D newOriginalSize)
{
	this->originalSize = newOriginalSize;
}

Vector2D Actor::getOriginalSize()
{
	return this->originalSize;
}

void Actor::setColideBox(BoundingBox newColideBox)
{
	this->colideBox = newColideBox;
}

void Actor::setGeometry(Hull newGeometry)
{
	this->initialGeometry = newGeometry;
}

void Actor::updateGeometry()
{
	Hull localGeometry;
	for (auto point : this->initialGeometry.points)
	{
		Vector2D scaled(point);
		scaled.x = point.x * this->scale.x;
		scaled.y = point.y * this->scale.y;

		Vector2D rotated = Vector2D(scaled.rotation() + this->direction).ort() * scaled.size();

		localGeometry.points.insert(localGeometry.points.end(), rotated);
	}
	localGeometry.generate();
	this->geometry = localGeometry;
}

void Actor::setClassId(std::string newClassId)
{
	this->classId = newClassId;
}

void Actor::setActorId(std::string newActorId)
{
	this->actorId = newActorId;
}

void Actor::onUpdateLocation() { }
void Actor::onUpdateRotation() { }
void Actor::onUpdateSize() { }