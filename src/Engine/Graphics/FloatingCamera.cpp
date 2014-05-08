#include "FloatingCamera.h"


FloatingCamera::FloatingCamera(HGE* hge, World* world, Vector2D resolution, Vector2D location) : Camera(hge, world, resolution, location), 
	shift(ZERO_VECTOR)
{
}


FloatingCamera::~FloatingCamera(void)
{
}

void FloatingCamera::setLocation(Vector2D newLocation)
{
	this->location = newLocation;
	this->centerPos = this->resolution/2 - this->shift;
}

void FloatingCamera::setCenterShift(Vector2D shift)
{
	this->shift = shift;
	this->centerPos = this->resolution/2 - this->shift;
}

void FloatingCamera::renderFog()
{
	this->fogSprite->RenderEx(this->resolution.x/2 - this->shift.x, this->resolution.y/2 - this->shift.y, 0, this->fogScale, this->fogScale);
	//Camera::RenderFog();
}
