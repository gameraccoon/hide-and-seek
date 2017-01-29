#pragma once

#include <memory>

#include "Core/ActorComponent.h"

/**
 * The base class for actor components
 *
 * Abstract
 */
class RenderComponent : public ActorComponent
{
public:
	using Ptr = std::shared_ptr<RenderComponent>;
	using WeakPtr = std::weak_ptr<RenderComponent>;

public:
	virtual ~RenderComponent() = default;

private:
	
};
