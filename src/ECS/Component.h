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

	virtual StringID getComponentTypeName() const = 0;
};
