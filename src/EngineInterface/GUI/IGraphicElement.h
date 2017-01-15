#pragma once

#include "Engine/Core/Vector2D.h"

class IGraphicElement
{
public:
	virtual ~IGraphicElement() { }
	virtual void render() const = 0;
	virtual Vector2D getScreenLocation() const = 0;
	virtual void setScreenLocation(const Vector2D& scrLocation) = 0;
	/**
	 * @return false if we missed
	 */
	virtual bool click() = 0;

	virtual bool checkHovered(const Vector2D& mousePos) = 0;
};
