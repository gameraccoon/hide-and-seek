#include "Core/Actor.h"

#include <sstream>
#include <memory>

namespace
{
	int ClassIndex = 0;
}

Actor::Ptr Actor::Create(World * world, Vector2D location, Rotator rotation)
{
	return std::make_unique<Actor>(world, location, rotation);
}

Actor::Actor(World *world, Vector2D location, Rotator rotation)
    : mOwnerWorld(world)
    , mType(ActorType::Ghost)
    , mLocation(location)
    , mScale(1.f, 1.f)
    , mOriginalSize(1.f, 1.f)
    , mCalculatedSize(1.f, 1.f)
    , mDirection(rotation)
    , mColideBox(location, location)
	, mIsWaitDestruction(false)
	, mLifetime(0)
{
	updateActorId("Actor");
}

Actor::~Actor()
{
}

void Actor::setLocation(const Vector2D& location)
{
	mLocation = Vector2D(location);
	updateCollision();
	onUpdateLocation();
}

Vector2D Actor::getLocation() const
{
	return mLocation;
}

void Actor::setRotation(const Rotator& rotation)
{
	mDirection = Rotator(rotation);
	updateCollision();
	onUpdateRotation();
	updateGeometry();
}

Rotator Actor::getRotation() const
{
	return mDirection;
}

void Actor::setScale(const Vector2D& scale)
{
	mScale = Vector2D(scale);
	mCalculatedSize.x = mOriginalSize.x * scale.x;
	mCalculatedSize.y = mOriginalSize.y * scale.y;
	onUpdateSize();
	updateGeometry();
}

Vector2D Actor::getScale() const
{
	return mScale;
}

ActorType Actor::getType() const
{
	return mType;
}

BoundingBox Actor::getBoundingBox() const
{
	return mColideBox;
}

const Hull* Actor::getGeometry() const
{
	return &(mGeometry);
}

std::string Actor::getClassID() const
{
	return mClassId;
}

std::string Actor::getActorId() const
{
	return mActorId;
}

void Actor::updateActorId(std::string classId)
{
	mClassId = classId;
	std::ostringstream s;
	s << ::ClassIndex++;
	mActorId = classId + s.str();
}

void Actor::update(float deltatime)
{
	mLifetime += deltatime;
}

void Actor::destroy()
{
	if (!mIsWaitDestruction)
	{
		mIsWaitDestruction = true;
	}
}

bool Actor::isWaitDestruction() const
{
	return mIsWaitDestruction;
}

void Actor::hit(IActor *, float , Vector2D )
{
}

void Actor::AddComponent(const ActorComponent::Ptr& component)
{
	mComponents.push_back(component);
}

World* Actor::getOwnerWorld() const
{
	return mOwnerWorld;
}

float Actor::getLifetime() const
{
	return mLifetime;
}

Vector2D Actor::getCalculatedSize() const
{
	return mCalculatedSize;
}

void Actor::setType(ActorType newType)
{
	mType = newType;
}

void Actor::setOriginalSize(Vector2D newOriginalSize)
{
	mOriginalSize = newOriginalSize;
}

Vector2D Actor::getOriginalSize()
{
	return mOriginalSize;
}

void Actor::setColideBox(BoundingBox newColideBox)
{
	mColideBox = newColideBox;
}

void Actor::updateCollision()
{
	if (getGeometry()->type == Hull::Type::Circular)
	{
		float radius = getGeometry()->getRadius();
		setColideBox(BoundingBox(getLocation() - Vector2D(radius, radius), getLocation() + Vector2D(radius, radius)));
	}
	else
	{
		float minX = 1000;
		float maxX = -1000;
		float minY = 1000;
		float maxY = -1000;

		for (auto point : mGeometry.points)
		{
			if (point.x < minX)
			{
				minX = point.x;
			}
		
			if (point.x > maxX)
			{
				maxX = point.x;
			}
		
			if (point.y < minY)
			{
				minY = point.y;
			}
		
			if (point.y > maxY)
			{
				maxY = point.y;
			}
		}

		setColideBox(BoundingBox(getLocation() + Vector2D(minX, minY), getLocation() + Vector2D(maxX, maxY)));
	}
}

void Actor::setGeometry(Hull newGeometry)
{
	mInitialGeometry = newGeometry;
}

void Actor::updateGeometry()
{
	Hull localGeometry(mInitialGeometry);
	localGeometry.points.clear();
	for (auto point : mInitialGeometry.points)
	{
		Vector2D scaled(point);
		scaled.x = point.x * mScale.x;
		scaled.y = point.y * mScale.y;

		Vector2D rotated = Vector2D(scaled.rotation() + mDirection).ort() * scaled.size();

		localGeometry.points.insert(localGeometry.points.end(), rotated);
	}
	localGeometry.generateBorders();
	mGeometry = localGeometry;

	updateCollision();
}

void Actor::setClassId(std::string newClassId)
{
	mClassId = newClassId;
}

void Actor::setActorId(std::string newActorId)
{
	mActorId = newActorId;
}

void Actor::onUpdateLocation() { }
void Actor::onUpdateRotation() { }
void Actor::onUpdateSize() { }
