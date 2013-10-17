#include "FloatingCamera.h"


FloatingCamera::FloatingCamera(World* world, Vector2D location) : Camera(world, location), Shift(ZeroVector)
{
}


FloatingCamera::~FloatingCamera(void)
{
}

void FloatingCamera::SetLocation(Vector2D newLocation)
{
	Location = newLocation;
	CenterPos = Resolution/2 - Shift;
}

void FloatingCamera::SetResolution(Vector2D newResolution)
{
	Resolution = newResolution;
	CenterPos = Resolution/2 - Shift;
}

void FloatingCamera::SetCenterShift(Vector2D shift)
{
	Shift = shift;
	CenterPos = Resolution/2 - Shift;
}