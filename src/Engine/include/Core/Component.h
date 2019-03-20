#pragma once

#include <memory>

/**
 * The base class for actor components
 *
 * Abstract
 */
class BaseComponent
{
public:
	using Ptr = std::shared_ptr<BaseComponent>;
	using ConstPtr = std::shared_ptr<const BaseComponent>;
	using WeakPtr = std::weak_ptr<BaseComponent>;

public:
	virtual ~BaseComponent() = default;
};
