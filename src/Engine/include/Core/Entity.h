#pragma once

#include <string>

class Entity
{
public:
	typedef unsigned int EntityID;

public:
	explicit Entity(EntityID id) : mId(id) {}

	EntityID getID() const { return mId; }

private:
	EntityID mId;
};

class NullableEntity
{
public:
	NullableEntity() = default;
	// implicit conversion
	NullableEntity(const Entity& entity) : mId(entity.getID()), mIsNull(false) {}
	explicit NullableEntity(Entity::EntityID id) : mId(id), mIsNull(false) {}

	bool isNull() const { return mIsNull; }
	bool isValid() const { return !mIsNull; }
	Entity getEntity() const { return Entity(mId); }

public:
	Entity::EntityID mId = 0;
	bool mIsNull = true;
};
