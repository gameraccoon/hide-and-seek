#pragma once

/**
 * The base class for actor components
 *
 * Abstract
 */
class BaseComponent
{
public:
	virtual ~BaseComponent() = default;

	virtual void toJson(nlohmann::json& outJson) const = 0;
	virtual void fromJson(const nlohmann::json& json) = 0;

	virtual StringID getComponentTypeName() const = 0;
};
