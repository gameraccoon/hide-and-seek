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
	CenterPos = Location + Shift - Resolution/2;
}

void FloatingCamera::SetResolution(Vector2D newResolution)
{
	Resolution = newResolution;
	CenterPos = Location + Shift - Resolution/2;
}

void FloatingCamera::SetCenterShift(Vector2D shift)
{
	Shift = shift;
	CenterPos = Location + Shift - Resolution/2;
}