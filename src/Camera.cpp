#include "Camera.h"


Camera::Camera(HGE *hge, World* world, Vector2D location) : Location(location),
												Resolution(ZeroVector),
												CenterPos(ZeroVector)
{
	BrowsableWorld = world;
	Angle = 0;

	Hge = hge;

	FogWidth = 512;
	FogScale = 1.5f / (FogWidth / 2.0f);

	CamTexture = Hge->Texture_Load("colision.png");
	FogTexture = Hge->Texture_Load("fog.png");

	WARN_IF(!CamTexture || !FogTexture, "Texture 'colision.png' or 'for.png' not found!");

	CollisionSprite = new hgeSprite(CamTexture, 0, 0, 64, 64);
	CollisionSprite->SetColor(0xFF00FF00);
	CollisionSprite->SetHotSpot(0, 0);

	FogSprite = new hgeSprite(FogTexture, 0.0f, 0.0f, FogWidth, FogWidth);
	FogSprite->SetColor(0xFF000000);
	FogSprite->SetHotSpot(FogWidth/2, FogWidth/2);

	ShownSize = 500.f;

	bShowCollision = false;
	bRenderFog = true;
}

Camera::~Camera(void)
{
	delete CollisionSprite;
	delete FogSprite;
	Hge->Texture_Free(CamTexture);
	Hge->Texture_Free(FogTexture);
}

void Camera::Render()
{
	for (std::set<IActor*>::iterator it = BrowsableWorld->AllActors.begin(); it != BrowsableWorld->AllActors.end(); it++)
	{
		Vector2D screenLoc((*it)->GetLocation() - Location);
		float distanceFromCamera = screenLoc.Size();
		if (distanceFromCamera < ShownSize)
		{
			Vector2D newScreenLoc(Vector2D(screenLoc.GetRotation() + Angle) * distanceFromCamera);
			(*it)->Render(CenterPos + newScreenLoc, Angle);
		}
	}

	if (bShowCollision)
		RenderCollisionBoxes();
	if (bRenderFog)
		RenderFog();
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
		CollisionSprite->RenderEx(shift.X, shift.Y, Angle, size.X/64.0f, size.Y/64.0f);
	}
}

void Camera::RenderFog()
{
	FogSprite->RenderEx(Resolution.X/2, Resolution.Y/2, 0, ShownSize * FogScale, ShownSize * FogScale);
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

void Camera::ShowCollision(bool bShow)
{
	bShowCollision = bShow;
}

void Camera::ShowFog(bool bShow)
{
	bRenderFog = bShow;
}