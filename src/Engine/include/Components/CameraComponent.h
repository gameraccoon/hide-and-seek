#pragma once

#include <memory>

#include "Core/Component.h"
#include "Core/Vector2D.h"

/**
 * Component that contains camera-relating information
 */
class CameraComponent : public BaseComponent
{
public:
	using Ptr = std::shared_ptr<CameraComponent>;
	using WeakPtr = std::weak_ptr<CameraComponent>;

public:
	virtual ~CameraComponent() = default;
};
