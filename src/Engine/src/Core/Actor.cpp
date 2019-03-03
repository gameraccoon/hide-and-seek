#include "Core/Actor.h"

#include <sstream>
#include <memory>

namespace
{
	int ClassIndex = 0;
}

Actor::Ptr Actor::Create(World * world)
{
	return std::make_unique<Actor>(world);
}

Actor::Actor(World *world)
    : mOwnerWorld(world)
    , mType(ActorType::Ghost)
	, mIsWaitDestruction(false)
	, mLifetime(0)
{
	updateActorId("Actor");
}

Actor::~Actor()
{
}

ActorType Actor::getType() const
{
	return mType;
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

void Actor::addComponent(const ActorComponent::Ptr& component)
{
	mComponents.push_back(component);
}

std::vector<ActorComponent::Ptr>&Actor::getAllComponents()
{
	return mComponents;
}

World* Actor::getOwnerWorld() const
{
	return mOwnerWorld;
}

float Actor::getLifetime() const
{
	return mLifetime;
}

void Actor::setType(ActorType newType)
{
	mType = newType;
}

void Actor::setClassId(std::string newClassId)
{
	mClassId = newClassId;
}

void Actor::setActorId(std::string newActorId)
{
	mActorId = newActorId;
}

