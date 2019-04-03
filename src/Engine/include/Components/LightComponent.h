#pragma once

#include <memory>

#include "Core/Component.h"
#include "Components/TransformComponent.h"
#include "Core/Vector2D.h"

/**
 * Component that stores information about a light emitter
 */
class LightComponent : public BaseComponent
{
public:
	virtual ~LightComponent() override = default;

	float getBrightness() const;
	void setBrightness(float newSize);

private:
	float mBrightness;
};
