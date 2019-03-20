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
	using Ptr = std::shared_ptr<LightComponent>;
	using WeakPtr = std::weak_ptr<LightComponent>;

public:
	virtual ~LightComponent() = default;

	float getBrightness() const;
	void setBrightness(float newSize);

private:
	float mBrightness;
};
