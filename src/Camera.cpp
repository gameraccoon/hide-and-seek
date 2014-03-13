#include "Camera.h"


Camera::Camera(HGE* hge, World* world, Vector2D resolution, Vector2D location) : location(location),
	resolution(resolution),
	centerPos(resolution / 2),
	graphicLoader(hge, std::string("./configs/asd.txt"), "./")
{
	this->browsableWorld = world;
	this->angle = 0;

	this->hge = hge;

	// Set max distantion (on screen) where we draw actors
	this->shownSize = 512.0f;

	// Set fog texture size
	this->fogWidth = 512.0f;
	// Set scale of fog sprite
	this->fogScale = this->shownSize * 0.9f / (this->fogWidth / 2.0f);

	this->renderTarget = this->hge->Target_Create((int) this->resolution.x, (int) this->resolution.y, false);
	this->zone = this->hge->Target_Create((int) this->shownSize, (int) this->shownSize, false);

	this->camTexture = this->hge->Texture_Load("colision.png");
	this->fogTexture = this->hge->Texture_Load("fog.png");

	WARN_IF(!this->camTexture || !this->fogTexture, "Texture 'colision.png' or 'for.png' not found!");

	this->collisionSprite = new hgeSprite(this->camTexture, 0, 0, 64, 64);
	this->collisionSprite->SetColor(0xFF00FF00);
	this->collisionSprite->SetHotSpot(0, 0);

	this->fogSprite = new hgeSprite(this->fogTexture, 0.0f, 0.0f, this->fogWidth, this->fogWidth);
	this->fogSprite->SetColor(0xFF000000);
	this->fogSprite->SetHotSpot(this->fogWidth/2, this->fogWidth/2);

	// Standart parameters of rendering
	bShowAABB = false;
	bRenderFog = true;
	bShowBorders = false;
	bRenderShadows = true;
	bShowLights = false;
	bShowPaths = false;
}

Camera::~Camera(void)
{
	delete this->collisionSprite;
	delete this->fogSprite;
	this->hge->Texture_Free(this->camTexture);
	this->hge->Texture_Free(this->fogTexture);
	this->hge->Target_Free(this->renderTarget);
	this->hge->Target_Free(this->zone);
}

void Camera::render()
{
	// clean lights cached map
	this->hge->Gfx_BeginScene(this->renderTarget);
	this->hge->Gfx_Clear(0);
	this->hge->Gfx_EndScene();

	// for each light on the scene
	for (std::set<IActor*>::iterator i = this->browsableWorld->allActors.begin(), iEnd = this->browsableWorld->allActors.end(); i != iEnd; i++)
	{
		if ((*i)->getType() == AT_Light)
		{
			this->hge->Gfx_BeginScene(this->zone);
			this->hge->Gfx_Clear(0xFFFFFF);
			
			// Actors beside this light
			this->renderActors((*i)->getLocation());

			// Shadows for this light
			if (this->bRenderShadows)
			{
				this->renderLightShadows((*i)->getLocation());
			}

			// Fog of this light
			if (this->bRenderFog)
			{
				this->fogSprite->RenderEx(this->shownSize/2, this->shownSize/2, 0, this->shownSize/this->fogWidth, this->shownSize/this->fogWidth);
			}
	
			this->hge->Gfx_EndScene();

			// render light to lights
			hgeSprite *light = new hgeSprite(this->hge->Target_GetTexture(this->zone), 0, 0, this->shownSize, this->shownSize);
			light->SetBlendMode(BLEND_COLORMUL);
			light->SetColor(0xFF777777);

			this->hge->Gfx_BeginScene(this->renderTarget);
			light->Render(this->project((*i)->getLocation()).x-this->shownSize/2, this->project((*i)->getLocation()).y - this->shownSize/2);
			this->hge->Gfx_EndScene();

			delete light;
		}
	}

	// start rendering to target
	this->hge->Gfx_BeginScene(this->renderTarget);

	// Shadows for player's view
	if (this->bRenderShadows)
	{
		//this->renderShadows();
	}

	// Render player's fog
	if (this->bRenderFog)
	{
		//this->renderFog();
	}

	// Bounding boxes
	if (this->bShowAABB)
	{
		this->renderCollisionBoxes();
	}

	// Borders
	if (this->bShowBorders)
	{
		this->renderHulls();
	}
	
	// Lights
	if (this->bShowLights)
	{
		this->renderLights();
	}

	// Paths
	if (this->bShowPaths)
	{
		this->renderPaths();
	}

	// end rendering to target
	this->hge->Gfx_EndScene();
}

void Camera::renderActors(Vector2D lightPos)
{
	// for each actors in the world
	for (std::set<IActor*>::iterator i = this->browsableWorld->allActors.begin(), iEnd = this->browsableWorld->allActors.end(); i != iEnd; i++)
	{
		IActor* actorToRender = *i;

		// Get actor's camera local location
		Vector2D screenLoc(actorToRender->getLocation() - lightPos);
		// Get distance from center of screen
		float distanceFromCamera = screenLoc.size();

		// if actor is not far of drawing zone
		if (distanceFromCamera >= this->shownSize)
			continue;

		// Get sprite
		hgeSprite* sprite = this->graphicLoader.getSprite(actorToRender->getClassID());

		// if there no sprite for this actor then go to the next actor
		if (sprite == NULL)
			continue;

		// calc actor screen location
		Vector2D newScreenLoc(Vector2D(screenLoc.rotation() + this->angle) * distanceFromCamera);
		// get scaled localtion
		Vector2D scaledLocation(newScreenLoc + Vector2D(this->shownSize/2, this->shownSize/2));

		// render actor
		sprite->RenderEx(scaledLocation.x, scaledLocation.y,
			(actorToRender->getRotation() + this->angle).getValue(), actorToRender->getScale().x, actorToRender->getScale().y);
	}
}

void Camera::renderCollisionBoxes()
{
	// for each actors in the world
	for (std::set<IActor*>::iterator i = this->browsableWorld->allActors.begin(), iEnd = this->browsableWorld->allActors.end(); i != iEnd; i++)
	{
		// get location and size of actor's AABB (axis-aligned bounding box)
		Vector2D min = (*i)->getBoundingBox().getFirst();
		Vector2D size = (*i)->getBoundingBox().getThird() - min;

		// project location on the screen
		Vector2D shift = this->project(min);

		// render collision sprite
		this->collisionSprite->RenderEx(shift.x, shift.y, this->angle, size.x/64.0f, size.y/64.0f);
	}
}

Vector2D Camera::project(Vector2D worldPoint)
{
	// calc camera-location
	Vector2D screenLoc(worldPoint - this->location);
	// calc screen-coordinates relativety camera center
	Vector2D newScreenLoc(Vector2D(screenLoc.rotation() + this->angle) * screenLoc.size());
	// calc absolute screen-coordinates
	return this->centerPos + newScreenLoc;
}

Vector2D Camera::projectFrom(Vector2D worldPoint, Vector2D projectionCenter)
{
	// calc camera-location
	Vector2D screenLoc(worldPoint - projectionCenter);
	// calc screen-coordinates relativety camera center
	Vector2D newScreenLoc(Vector2D(screenLoc.rotation() + this->angle) * screenLoc.size());
	// calc absolute screen-coordinates
	return Vector2D(this->shownSize/2, this->shownSize/2) + newScreenLoc;
}

Vector2D Camera::deProject(Vector2D screenPoint)
{
	// calc relative screen-coordinates
	Vector2D relScreenLoc(screenPoint - this->centerPos);
	// calc world-coordinates relativety camera
	Vector2D screenLoc = Vector2D(relScreenLoc.rotation() - this->angle) * relScreenLoc.size();
	// calc world location
	return this->location + screenLoc;
}

void Camera::renderFog()
{
	// render fog sprite
	this->fogSprite->RenderEx(this->resolution.x/2, this->resolution.y/2, 0, this->fogScale, this->fogScale);
}

void Camera::drawPenumbra(Vector2D first, Vector2D second, Vector2D third)
{
	HTEXTURE PenumbraTexture = this->hge->Texture_Load("penumbra.png");

	WARN_IF(!PenumbraTexture, "Texture 'penumbra.png' not found!");
	
	hgeTriple triple;
	//triple.
	triple.tex = PenumbraTexture;
	triple.blend = BLEND_DEFAULT;
	triple.v[0].z = triple.v[1].z = triple.v[2].z = 0;
	triple.v[0].x = 100;
	triple.v[0].y = 200;
	triple.v[1].x = 300;
	triple.v[1].y = 400;
	triple.v[2].x = 100;
	triple.v[2].y = 400;
	
	triple.v[0].col = 0xFFFFFFFF;
	triple.v[0].tx = 1;
	triple.v[0].ty = 1;
	
	triple.v[1].col = 0xFFFFFFFF;
	triple.v[1].tx = 0;
	triple.v[1].ty = 1;
	
	triple.v[2].col = 0xFFFFFFFF;
	triple.v[2].tx = 1;
	triple.v[2].ty = 0;

	this->hge->Gfx_RenderTriple(&triple);
}

void Camera::drawQuad(Vector2D first, Vector2D second, Vector2D third, Vector2D fourth)
{
	hgeQuad quad;
	quad.tex = 0;
	quad.blend = BLEND_DEFAULT;
	quad.v[0].z = quad.v[1].z = quad.v[2].z = quad.v[3].z = 0.5f;
	quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = 0xFF000000;

	quad.v[0].x = first.x;
	quad.v[0].y = first.y;
		
	quad.v[1].x = second.x;
	quad.v[1].y = second.y;
		
	quad.v[2].x = third.x;
	quad.v[2].y = third.y;
		
	quad.v[3].x = fourth.x;
	quad.v[3].y = fourth.y;

	this->hge->Gfx_RenderQuad(&quad);
}

void Camera::renderLightShadows(Vector2D lightPos)
{
	Vector2D lightCenterPos(this->shownSize/2, this->shownSize/2);
	// for each actors in the world
	for (std::set<IActor*>::iterator i = this->browsableWorld->allActors.begin(), iEnd = this->browsableWorld->allActors.end(); i != iEnd; i++)
	{
		// if actor - static
		if ((*i)->getType() == AT_Static)
		{
			// get actors geometry
			Hull *hull = (*i)->getHull();
			// for each border of actor's geometry
			for (int j = 0; j < (int) hull->borders.size(); j++)
			{
				// if border's normal and camera view on border have different directions (angle > PI/2)
				if (abs((
						hull->borders[j].getNormal().rotation()
						- ((*i)->getLocation() + (hull->borders[j].getA() + hull->borders[j].getB())/2 - lightPos).rotation()
					).getValue()) < PI/2)
				{
					// Get border's points
					Vector2D a((*i)->getLocation() + hull->borders[j].getA());
					Vector2D b((*i)->getLocation() + hull->borders[j].getB());

					// project them on screen
					a = this->projectFrom(a, lightPos);
					b = this->projectFrom(b, lightPos);

					// Draw shadow's quad of this border to far of the screen
					this->drawQuad(a, b, b + (b - lightCenterPos).ort() * 3000, a + (a - lightCenterPos).ort() * 3000);
				}
			}
		}
	}
}

void Camera::renderShadows()
{
	// for each actors in the world
	for (std::set<IActor*>::iterator i = this->browsableWorld->allActors.begin(), iEnd = this->browsableWorld->allActors.end(); i != iEnd; i++)
	{
		// if actor - static
		if ((*i)->getType() == AT_Static)
		{
			// get actors geometry
			Hull *hull = (*i)->getHull();
			// for each border of actor's geometry
			for (int j = 0; j < (int) hull->borders.size(); j++)
			{
				// if border's normal and camera view on border have different directions (angle > PI/2)
				if (abs((
						hull->borders[j].getNormal().rotation()
						- ((*i)->getLocation() + (hull->borders[j].getA() + hull->borders[j].getB())/2 - this->location).rotation()
					).getValue()) < PI/2)
				{
					// Get border's points
					Vector2D a((*i)->getLocation() + hull->borders[j].getA());
					Vector2D b((*i)->getLocation() + hull->borders[j].getB());

					// project them on screen
					a = this->project(a);
					b = this->project(b);

					// Draw shadow's quad of this border to far of the screen
					this->drawQuad(a, b, b + (b - this->centerPos).ort() * 3000, a + (a - this->centerPos).ort() * 3000);
				}
			}
		}
	}
}

void Camera::renderHulls()
{
	const int normal_length = 10;

	// for each actors in the world
	for (std::set<IActor*>::iterator i = this->browsableWorld->allActors.begin(), iEnd = this->browsableWorld->allActors.end(); i != iEnd; i++)
	{
		// if actor - static
		if ((*i)->getType() != AT_Light && (*i)->getType() != AT_Special)
		{
			// get actors geometry
			Hull *hull = (*i)->getHull();
			// for each border of actor's geometry
			for (int j = 0; j < (int) hull->borders.size(); j++)
			{
				// draw border
				Vector2D A((*i)->getLocation() + hull->borders[j].getA()), B((*i)->getLocation() + hull->borders[j].getB());
				A = this->project(A);
				B = this->project(B);
				
				this->hge->Gfx_RenderLine(A.x, A.y, B.x, B.y, 0xFFFF00FF);

				// draw normal
				Vector2D LinePos((*i)->getLocation() + (hull->borders[j].getA() + hull->borders[j].getB()) / 2);
				Vector2D Norm(LinePos + hull->borders[j].getNormal() * normal_length);

				LinePos = this->project(LinePos);
				Norm = this->project(Norm);

				this->hge->Gfx_RenderLine(LinePos.x, LinePos.y, Norm.x, Norm.y, 0xFF0000FF);
			}
		}
	}
}

void Camera::renderLights()
{
	// for each actors in the world
	for (std::set<IActor*>::iterator i = this->browsableWorld->allActors.begin(), iEnd = this->browsableWorld->allActors.end(); i != iEnd; i++)
	{
		// if actor - static
		if ((*i)->getType() == AT_Light)
		{
			Vector2D lightLocation(this->project((*i)->getLocation()));
			this->hge->Gfx_RenderLine(lightLocation.x - 5, lightLocation.y, lightLocation.x + 5, lightLocation.y, 0xFFAA6600, 0);
			this->hge->Gfx_RenderLine(lightLocation.x, lightLocation.y - 5, lightLocation.x, lightLocation.y + 5, 0xFFAA6600, 0);
		}
	}
}

void Camera::renderPaths()
{
	// for each actors in the world
	for (std::set<PathPoint*>::iterator i = this->browsableWorld->navigationMap.begin(),
		iEnd = this->browsableWorld->navigationMap.end(); i != iEnd; i++)
	{
		Vector2D pathPointLocation(this->project((*i)->location));
		
		// render green diamond
		this->hge->Gfx_RenderLine(pathPointLocation.x - 5, pathPointLocation.y, pathPointLocation.x, pathPointLocation.y - 5, 0xFF00FF00, 0);
		this->hge->Gfx_RenderLine(pathPointLocation.x, pathPointLocation.y - 5, pathPointLocation.x + 5, pathPointLocation.y, 0xFF00FF00, 0);
		this->hge->Gfx_RenderLine(pathPointLocation.x + 5, pathPointLocation.y, pathPointLocation.x, pathPointLocation.y + 5, 0xFF00FF00, 0);
		this->hge->Gfx_RenderLine(pathPointLocation.x, pathPointLocation.y + 5, pathPointLocation.x - 5, pathPointLocation.y, 0xFF00FF00, 0);

		// render path arrow
		for (std::set<PathPoint*>::iterator j = (*i)->legalPoints.begin(), jEnd = (*i)->legalPoints.end(); j != jEnd; j++)
		{
			Vector2D nextPointLocation(this->project((*j)->location));
			this->hge->Gfx_RenderLine(pathPointLocation.x, pathPointLocation.y, nextPointLocation.x, nextPointLocation.y, 0xFF00FF00, 0);
			Vector2D thirdPoint = pathPointLocation + (nextPointLocation - pathPointLocation)/3;

			Vector2D Direction = (nextPointLocation - pathPointLocation).ort();
			float arrowlength = 5.f;
			
			this->hge->Gfx_RenderLine(thirdPoint.x,
				thirdPoint.y,
				thirdPoint.x + (Direction.normal().x - Direction.ort().x) * arrowlength,
				thirdPoint.y + (Direction.normal().y - Direction.ort().y) * arrowlength, 0xFF00FF00, 0);
			
			this->hge->Gfx_RenderLine(thirdPoint.x,
				thirdPoint.y,
				thirdPoint.x - (Direction.normal().x + Direction.ort().x) * arrowlength,
				thirdPoint.y - (Direction.normal().y + Direction.ort().y) * arrowlength, 0xFF00FF00, 0);
		}
	}
}

void Camera::setLocation(Vector2D newLocation)
{
	this->location = newLocation;
}

Vector2D Camera::getResolution()
{
	return this->resolution;
}

void Camera::setRotation(Rotator angle)
{
	this->angle = angle.getValue();
}

void Camera::showAABB(bool show)
{
	this->bShowAABB = show;
}

void Camera::showFog(bool show)
{
	this->bRenderFog = show;
}

void Camera::showShadows(bool show)
{
	this->bRenderShadows = show;
}

void Camera::showHulls(bool show)
{
	this->bShowBorders = show;
}

void Camera::showLights(bool show)
{
	this->bShowLights = show;
}

void Camera::showPaths(bool show)
{
	this->bShowPaths = show;
}

HTEXTURE Camera::getRenderTexture()
{
	return this->renderTarget;
}
