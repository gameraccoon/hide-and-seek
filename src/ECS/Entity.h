#pragma once

#include <nlohmann/json_fwd.hpp>

class Entity
{
public:
	using EntityID = unsigned int;

public:
	explicit Entity(EntityID id) : mId(id) {}

	bool operator ==(Entity b) const { return mId == b.mId; }
	bool operator !=(Entity b) const { return !(*this == b); }
	bool operator <(Entity b) const { return mId < b.mId; }

	EntityID getID() const { return mId; }

	friend void to_json(nlohmann::json& outJson, Entity entity);
	friend void from_json(const nlohmann::json& json, Entity& outEntity);

private:
	EntityID mId;
};

class OptionalEntity
{
public:
	OptionalEntity() = default;
	// implicit conversion
	OptionalEntity(Entity entity) : mId(entity.getID()), mIsValid(true) {}
	explicit OptionalEntity(Entity::EntityID id) : mId(id), mIsValid(true) {}

	bool isValid() const { return mIsValid; }
	Entity getEntity() const noexcept;
	Entity::EntityID getID() const noexcept;

	friend void to_json(nlohmann::json& outJson, const OptionalEntity& entity);
	friend void from_json(const nlohmann::json& json, OptionalEntity& outEntity);

private:
	Entity::EntityID mId = 0;
	bool mIsValid = false;
};
