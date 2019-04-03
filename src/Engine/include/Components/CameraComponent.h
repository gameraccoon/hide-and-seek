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
	virtual ~CameraComponent() override = default;
};
