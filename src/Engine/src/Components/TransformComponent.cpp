#include "Components/TransformComponent.h"

#include <nlohmann/json.hpp>

TransformComponent::TransformComponent()
	: mLocation(0.0f, 0.0f)
	, mRotation(0.0f)
{
}

TransformComponent::TransformComponent(const Vector2D& location, const Rotator& rotation)
	: mLocation(location)
	, mRotation(rotation)
{
}

Vector2D TransformComponent::getLocation() const
{
	return mLocation;
}

void TransformComponent::setLocation(const Vector2D& newLocation)
{
	mLocation = newLocation;
}

void TransformComponent::shiftLocation(const Vector2D& deltaLocation)
{
	mLocation += deltaLocation;
}

Rotator TransformComponent::getRotation() const
{
	return mRotation;
}

void TransformComponent::setRotation(const Rotator& newRotation)
{
	mRotation = newRotation;
}

void TransformComponent::shiftRotation(const Rotator& deltaRotation)
{
	mRotation += deltaRotation;
}

void TransformComponent::toJson(nlohmann::json& outJson) const
{
	to_json(outJson, *this);
}

void TransformComponent::fromJson(const nlohmann::json& json)
{
	from_json(json, *this);
}

void to_json(nlohmann::json& outJson, const TransformComponent& transform)
{
	outJson = nlohmann::json{{"loc", transform.mLocation}, {"rot", transform.mRotation}};
}

void from_json(const nlohmann::json& json, TransformComponent& outTransform)
{
	json.at("loc").get_to(outTransform.mLocation);
	json.at("rot").get_to(outTransform.mRotation);
}
