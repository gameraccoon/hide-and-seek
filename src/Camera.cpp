#include "Camera.h"


Camera::Camera(World* world, Vector2D location) : Location(location),
												Resolution(ZeroVector),
												CenterPos(ZeroVector)
{
	BrowsableWorld = world;
	Angle = 0;
}

Camera::~Camera(void)
{
}

void Camera::Render()
{
	for (std::set<Actor*>::iterator it = BrowsableWorld->AllActors.begin(); it != BrowsableWorld->AllActors.end(); it++)
	{
		Vector2D screenLoc(- Location + (*it)->GetLocation());
		Vector2D newScreenLoc(Vector2D(screenLoc.GetRotation() + Angle) * screenLoc.Size());
		(*it)->Render(CenterPos + newScreenLoc, Angle);
	}
}

void Camera::SetLocation(Vector2D newLocation)
{
	Location = newLocation;
}

void Camera::SetResolution(Vector2D newResolution)
{
	Resolution = newResolution;
	CenterPos = Resolution/2;
}

Vector2D Camera::GetResolution()
{
	return Resolution;
}

void Camera::SetRotation(Rotator angle)
{
	Angle = angle;
}