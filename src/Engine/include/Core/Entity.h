#pragma once

#include <string>
#include <Log.h>
#include <Assert.h>

class Entity
{
public:
	typedef unsigned int EntityID;

public:
	explicit Entity(EntityID id) : mId(id) {}

	bool operator ==(const Entity& b) const { return mId == b.mId; }
	bool operator !=(const Entity& b) const { return !(*this == b); }
	bool operator <(const Entity& b) const { return mId < b.mId; }

	EntityID getID() const { return mId; }

private:
	EntityID mId;
};

class NullableEntity
{
public:
	NullableEntity() = default;
	// implicit conversion
	NullableEntity(const Entity& entity) : mId(entity.getID()), mIsValid(true) {}
	explicit NullableEntity(Entity::EntityID id) : mId(id), mIsValid(true) {}

	bool operator ==(const NullableEntity& b) const
	{
		return mIsValid && b.mIsValid && mId == b.mId;
	}

	bool operator !=(const NullableEntity& b) const { return !(*this == b); }

	bool isValid() const { return mIsValid; }
	Entity getEntity() const {
		Assert(mIsValid, "Getting uninitialized entity");
		return Entity(mId);
	}

public:
	Entity::EntityID mId = 0;
	bool mIsValid = false;
};
