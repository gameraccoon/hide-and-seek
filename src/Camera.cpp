#include "Camera.h"


Camera::Camera(HGE *hge, World* world, Vector2D location) : Location(location),
												Resolution(ZeroVector),
												CenterPos(ZeroVector)
{
	BrowsableWorld = world;
	Angle = 0;

	Hge = hge;

	CamTexture = Hge->Texture_Load("colizion.png");

	WARN_IF(!CamTexture, "Texture 'colizion.png' not found!");
		
	Sprite = new hgeSprite(CamTexture, 0, 0, 64, 64);
	Sprite->SetColor(0xFF00FF00);
	Sprite->SetHotSpot(0, 0);

	bShowCollizion = false;
}

Camera::~Camera(void)
{
	delete Sprite;
	Hge->Texture_Free(CamTexture);
}

void Camera::Render()
{
	for (std::set<IActor*>::iterator it = BrowsableWorld->AllActors.begin(); it != BrowsableWorld->AllActors.end(); it++)
	{
		Vector2D screenLoc((*it)->GetLocation() - Location);
		Vector2D newScreenLoc(Vector2D(screenLoc.GetRotation() + Angle) * screenLoc.Size());
		(*it)->Render(CenterPos + newScreenLoc, Angle);
	}

	if (bShowCollizion)RenderCollisionBoxes();
}

void Camera::RenderCollisionBoxes()
{
	for (std::set<IActor*>::iterator it = BrowsableWorld->AllActors.begin(); it != BrowsableWorld->AllActors.end(); it++)
	{
		Vector2D min = (*it)->GetBoundingBox().GetMin();
		Vector2D size = (*it)->GetBoundingBox().GetMax() - min;

		Vector2D screenLoc(min - Location);
		Vector2D newScreenLoc(Vector2D(screenLoc.GetRotation() + Angle) * screenLoc.Size());
		Vector2D shift = CenterPos + newScreenLoc;
		Sprite->RenderEx(shift.X, shift.Y, Angle, size.X/64.0f, size.Y/64.0f);
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

void Camera::ShowCollizion(bool bShow)
{
	bShowCollizion = bShow;
}