#include "Camera.h"


Camera::Camera(World* world, Vector2D location) : Location(location),
												Resolution(ZeroVector),
												CenterPos(ZeroVector)
{
	BrowsableWorld = world;
}

Camera::~Camera(void)
{
}

void Camera::Render()
{
	for (std::set<Actor*>::iterator it = BrowsableWorld->AllActors.begin(); it != BrowsableWorld->AllActors.end(); it++)
	{
		(*it)->Render(CenterPos);
	}
}

void Camera::SetLocation(Vector2D newLocation)
{
	Location = newLocation;
	CenterPos = Location - Resolution/2;
}

void Camera::SetResolution(Vector2D newResolution)
{
	Resolution = newResolution;
	CenterPos = Location - Resolution/2;
}

Vector2D Camera::GetResolution()
{
	return Resolution;
}