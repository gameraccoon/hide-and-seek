#include "Camera.h"


Camera::Camera(World* world, Vector2D location) : Location(location),
												Resolution(ZeroVector),
												CenterPos(ZeroVector)
{
	BrowsableWorld = world;
	Angle = 0;

	Hge = world->GetHge();

	// Set max distantion (on screen) where we draw actors
	ShownSize = 500.0f;

	// Set fog texture size
	FogWidth = 512.0f;
	// Set scale of fog sprite
	FogScale = ShownSize * 1.5f / (FogWidth / 2.0f);

	CamTexture = Hge->Texture_Load("colision.png");
	FogTexture = Hge->Texture_Load("fog.png");

	WARN_IF(!CamTexture || !FogTexture, "Texture 'colision.png' or 'for.png' not found!");

	CollisionSprite = new hgeSprite(CamTexture, 0, 0, 64, 64);
	CollisionSprite->SetColor(0xFF00FF00);
	CollisionSprite->SetHotSpot(0, 0);

	FogSprite = new hgeSprite(FogTexture, 0.0f, 0.0f, FogWidth, FogWidth);
	FogSprite->SetColor(0xFF000000);
	FogSprite->SetHotSpot(FogWidth/2, FogWidth/2);

	// Standart parameters of rendering
	bShowAABB = false;
	bRenderFog = true;
	bShowBorders = false;
	bRenderShadows = true;
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
	if (bRenderShadows)
		RenderShadows();

	RenderActors();

	if (bRenderFog)
		RenderFog();
	if (bShowAABB)
		RenderCollisionBoxes();
	if (bShowBorders)
		RenderHulls();
}

void Camera::RenderActors()
{
	// for each actors in the world
	for (std::set<IActor*>::iterator it = BrowsableWorld->AllActors.begin(); it != BrowsableWorld->AllActors.end(); it++)
	{
		// Get screen location
		Vector2D screenLoc((*it)->GetLocation() - Location);
		// Get distance from center of screen
		float distanceFromCamera = screenLoc.Size();
		// if actor is not far of drawing zone
		if (distanceFromCamera < ShownSize)
		{
			// calc real screen location
			Vector2D newScreenLoc(Vector2D(screenLoc.GetRotation() + Angle) * distanceFromCamera);
			// render actor
			(*it)->Render(CenterPos + newScreenLoc, Angle);
		}
	}
}

void Camera::RenderCollisionBoxes()
{
	// for each actors in the world
	for (std::set<IActor*>::iterator it = BrowsableWorld->AllActors.begin(); it != BrowsableWorld->AllActors.end(); it++)
	{
		// get location and size of actor's AABB (axis-aligned bounding box)
		Vector2D min = (*it)->GetBoundingBox().GetFirst();
		Vector2D size = (*it)->GetBoundingBox().GetThird() - min;

		// project location on the screen
		Vector2D shift = Project(min);

		// render collision sprite
		CollisionSprite->RenderEx(shift.X, shift.Y, Angle, size.X/64.0f, size.Y/64.0f);
	}
}

Vector2D Camera::Project(Vector2D point)
{
	// calc camera-location
	Vector2D screenLoc(point - Location);
	// calc screen-coordinates relativety camera center
	Vector2D newScreenLoc(Vector2D(screenLoc.GetRotation() + Angle) * screenLoc.Size());
	// calc absolute screen-coordinates
	return CenterPos + newScreenLoc;
}

void Camera::RenderFog()
{
	// render fog sprite
	FogSprite->RenderEx(Resolution.X/2, Resolution.Y/2, 0, FogScale, FogScale);
}

void Camera::DrawQuad(Vector2D first, Vector2D second, Vector2D third, Vector2D fourth)
{
	hgeQuad quad;
	quad.tex = 0;
	quad.blend = BLEND_DEFAULT;
	quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.5f;
	quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = 0xFF000000;

	quad.v[0].x = first.X;
	quad.v[0].y = first.Y;
		
	quad.v[1].x = second.X;
	quad.v[1].y = second.Y;
		
	quad.v[2].x = third.X;
	quad.v[2].y = third.Y;
		
	quad.v[3].x = fourth.X;
	quad.v[3].y = fourth.Y;

	Hge->Gfx_RenderQuad(&quad);
}

void Camera::RenderShadows()
{
	// for each actors in the world
	for (std::set<IActor*>::iterator it = BrowsableWorld->AllActors.begin(); it != BrowsableWorld->AllActors.end(); it++)
	{
		// if actor - static
		if ((*it)->GetType() == AT_Static)
		{
			// get actors geometry
			Hull *hull = (*it)->GetHull();
			// for each border of actor's geometry
			for (int i = 0; i < hull->Borders.size(); i++)
			{
				// if border's normal and camera view on border have different directions (angle > PI/2)
				if (abs((
						hull->Borders[i].GetNormal().GetRotation()
						- ((*it)->GetLocation() + (hull->Borders[i].GetA() + hull->Borders[i].GetB())/2 - Location).GetRotation()
					).GetValue()) > PI/2)
				{
					// Get border's points
					Vector2D a((*it)->GetLocation() + hull->Borders[i].GetA());
					Vector2D b((*it)->GetLocation() + hull->Borders[i].GetB());

					// project them on screen
					a = Project(a);
					b = Project(b);

					// Draw shadow's quad of this border to far of the screen
					DrawQuad(a, b, b + (b - CenterPos).Ort() * 3000, a + (a - CenterPos).Ort() * 3000);
				}
			}
		}
	}
}

void Camera::RenderHulls()
{
	const int normal_length = 10;

	// for each actors in the world
	for (std::set<IActor*>::iterator it = BrowsableWorld->AllActors.begin(); it != BrowsableWorld->AllActors.end(); it++)
	{
		// if actor - static
		if ((*it)->GetType() == AT_Static)
		{
			// get actors geometry
			Hull *hull = (*it)->GetHull();
			// for each border of actor's geometry
			for (int i = 0; i < hull->Borders.size(); i++)
			{
				// draw border
				Vector2D A((*it)->GetLocation() + hull->Borders[i].GetA()), B((*it)->GetLocation() + hull->Borders[i].GetB());
				A = Project(A);
				B = Project(B);
				
				Hge->Gfx_RenderLine(A.X, A.Y, B.X, B.Y, 0xFFFF00FF);

				// draw normal
				Vector2D LinePos((*it)->GetLocation() + (hull->Borders[i].GetA() + hull->Borders[i].GetB()) / 2);
				Vector2D Norm(LinePos + hull->Borders[i].GetNormal() * normal_length);

				LinePos = Project(LinePos);
				Norm = Project(Norm);

				Hge->Gfx_RenderLine(LinePos.X, LinePos.Y, Norm.X, Norm.Y, 0xFF0000FF);
			}
		}
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
	Angle = angle.GetValue();
}

void Camera::ShowAABB(bool bShow)
{
	bShowAABB = bShow;
}

void Camera::ShowFog(bool bShow)
{
	bRenderFog = bShow;
}

void Camera::ShowShadows(bool bShow)
{
	bRenderShadows = bShow;
}

void Camera::ShowHulls(bool bShow)
{
	bShowBorders = bShow;
}