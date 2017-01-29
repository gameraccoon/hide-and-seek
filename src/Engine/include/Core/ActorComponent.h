#pragma once

#include <memory>

/**
 * The base class for actor components
 *
 * Abstract
 */
class ActorComponent
{
public:
	using Ptr = std::shared_ptr<ActorComponent>;
	using WeakPtr = std::weak_ptr<ActorComponent>;

public:
	virtual ~ActorComponent() = 0;
};
