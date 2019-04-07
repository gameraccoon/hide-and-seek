#pragma once

#include <nlohmann/json_fwd.hpp>

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
};
