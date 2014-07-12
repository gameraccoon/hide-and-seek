#include "Camera.h"


Camera::Camera(HGE* hge, const World* world, Vector2D resolution, Vector2D location) : location(location),
	resolution(resolution),
	centerPos(resolution / 2)
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

	this->collisionSprite = GraphicLoader::Instance().getSprite("collision");
	this->collisionSprite->SetColor(0xFF00FF00);

	this->fogSprite = GraphicLoader::Instance().getSprite("fog");
	this->fogSprite->SetTextureRect(0.0f, 0.0f, this->fogWidth, this->fogWidth);
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
	for (auto const &actor : this->browsableWorld->allActors)
	{
		if (actor->getType() == ActorType::Light)
		{
			
			LightEmitter *light = dynamic_cast<LightEmitter*>(actor);
			if (light == nullptr)
				continue;

			this->hge->Gfx_BeginScene(this->zone); // start render to texture
			this->hge->Gfx_Clear(0xFFFFFF);
			
			// actors beside this light
			this->renderActors(light);

			// shadows for this light
			if (this->bRenderShadows)
			{
				this->renderLightShadows(light);
			}

			// fog of this light
			this->renderFog(this->shownSize, this->shownSize, this->shownSize / this->fogWidth * light->getBrightness());

			this->hge->Gfx_EndScene(); // end render to texture

			// prepare the sprite
			hgeSprite *lightSprite = new hgeSprite(this->hge->Target_GetTexture(this->zone), 0, 0, this->shownSize, this->shownSize);
			lightSprite->SetBlendMode(BLEND_COLORMUL);
			lightSprite->SetColor(0xFF777777);

			Vector2D projected = this->project(actor->getLocation());

			// render sprite to frame texture
			this->hge->Gfx_BeginScene(this->renderTarget);
			lightSprite->Render(projected.x-this->shownSize/2, projected.y - this->shownSize/2);
			this->hge->Gfx_EndScene();

			delete lightSprite;
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
		this->renderFog(this->resolution.x, this->resolution.y, fogScale);
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

void Camera::renderActors(const LightEmitter *light)
{
	const Vector2D lightPos = light->getLocation();
	const float lightBrightness = light->getBrightness();
	// for each actors in the world
	for (auto const &actorToRender : this->browsableWorld->allActors)
	{
		// Get actor's camera local location
		Vector2D screenLoc(actorToRender->getLocation() - lightPos);
		// Get distance from center of screen
		float distanceFromCamera = screenLoc.size();

		// if actor is not far of drawing zone
		if (distanceFromCamera >= this->shownSize * lightBrightness)
			continue;

		// Get sprite
		hgeSprite* sprite = GraphicLoader::Instance().getSprite(actorToRender->getClassID());

		// if there no sprite for this actor then go to the next actor
		if (sprite == nullptr)
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
	for (auto const &actor : this->browsableWorld->allActors)
	{
		// get location and size of actor's AABB (axis-aligned bounding box)
		Vector2D min = actor->getBoundingBox().getFirst();
		Vector2D size = actor->getBoundingBox().getThird() - min;

		// project location on the screen
		Vector2D shift = this->project(min);

		// render collision sprite
		this->collisionSprite->RenderEx(shift.x, shift.y, this->angle, size.x/64.0f, size.y/64.0f);
	}
}

Vector2D Camera::project(const Vector2D &worldPoint) const
{
	// calc camera-location
	Vector2D screenLoc(worldPoint - this->location);
	// calc screen-coordinates relativety camera center
	Vector2D newScreenLoc(Vector2D(screenLoc.rotation() + this->angle) * screenLoc.size());
	// calc absolute screen-coordinates
	return this->centerPos + newScreenLoc;
}

Vector2D Camera::projectFrom(const Vector2D &worldPoint, const Vector2D &projectionCenter) const
{
	// calc camera-location
	Vector2D screenLoc(worldPoint - projectionCenter);
	// calc screen-coordinates relativety camera center
	Vector2D newScreenLoc(Vector2D(screenLoc.rotation() + this->angle) * screenLoc.size());
	// calc absolute screen-coordinates
	return Vector2D(this->shownSize/2, this->shownSize/2) + newScreenLoc;
}

Vector2D Camera::deProject(const Vector2D &screenPoint) const
{
	// calc relative screen-coordinates
	Vector2D relScreenLoc(screenPoint - this->centerPos);
	// calc world-coordinates relativety camera
	Vector2D screenLoc = Vector2D(relScreenLoc.rotation() - this->angle) * relScreenLoc.size();
	// calc world location
	return this->location + screenLoc;
}

void Camera::renderFog(float width, float height, float size)
{
	// render fog sprite
	this->fogSprite->RenderEx(width/2, height/2, 0, size, size);

	// left shadow quad
	this->drawQuad(Vector2D(-2000.f, -2000.f), Vector2D(-2000.f, 2000.f),
		Vector2D(width/2 - size*fogWidth/2, 2000.f),
		Vector2D(width/2 - size*fogWidth/2, -2000.f));

	// right shadow quad
	this->drawQuad(Vector2D(2000.f, -2000.f), Vector2D(2000.f, 2000.f),
		Vector2D(width/2 + size*fogWidth/2, 2000.f),
		Vector2D(width/2 + size*fogWidth/2, -2000.f));

	// up shadow quad
	this->drawQuad(Vector2D(-2000.f, -2000.f), Vector2D(2000.f, -2000.f),
		Vector2D(2000.f, height/2 - size*fogWidth/2),
		Vector2D(-2000.f, height/2 - size*fogWidth/2));

	// down shadow quad
	this->drawQuad(Vector2D(-2000.f, 2000.f), Vector2D(2000.f, 2000.f),
		Vector2D(2000.f, height/2 + size*fogWidth/2),
		Vector2D(-2000.f, height/2 + size*fogWidth/2));
}

void Camera::drawPenumbra(const Vector2D &first, const Vector2D &second, const Vector2D &third)
{
	HTEXTURE PenumbraTexture = GraphicLoader::Instance().getTexture("penumbra.png");
	
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

void Camera::drawQuad(const Vector2D &first, const Vector2D &second, const Vector2D &third, const Vector2D &fourth)
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

void Camera::renderLightShadows(const LightEmitter *light)
{
	const Vector2D lightPos = light->getLocation();
	const float lightBrightness = light->getBrightness();

	Vector2D lightCenterPos(this->shownSize/2, this->shownSize/2);
	// for each actors in the world
	for (auto const &actor : this->browsableWorld->allActors)
	{
		// if actor - static
		if (actor->getType() == ActorType::Static)
		{
			// Get actor's camera local location
			Vector2D screenLoc(actor->getLocation() - lightPos);
			// Get distance from center of screen
			float distanceFromCamera = screenLoc.size();

			// if actor is not far of drawing zone
			if (distanceFromCamera >= this->shownSize * lightBrightness)
				continue;

			// get actors geometry
			const Hull *hull = actor->getGeometry();
			// for each border of actor's geometry
			for (int j = 0; j < (int) hull->borders.size(); j++)
			{
				// if border's normal and camera view on border have different directions (angle > PI/2)
				if (abs((
						hull->borders[j].getNormal().rotation()
						- (actor->getLocation() + (hull->borders[j].getA() + hull->borders[j].getB())/2 - lightPos).rotation()
					).getValue()) < PI/2)
				{
					// Get border's points
					Vector2D a(actor->getLocation() + hull->borders[j].getA());
					Vector2D b(actor->getLocation() + hull->borders[j].getB());

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
	for (auto const &actor : this->browsableWorld->allActors)
	{
		// if actor - static
		if (actor->getType() == ActorType::Static)
		{
			// get actors geometry
			const Hull *hull = actor->getGeometry();
			// for each border of actor's geometry
			for (int j = 0; j < (int) hull->borders.size(); j++)
			{
				// if border's normal and camera view on border have different directions (angle > PI/2)
				if (abs((
						hull->borders[j].getNormal().rotation()
						- (actor->getLocation() + (hull->borders[j].getA() + hull->borders[j].getB())/2 - this->location).rotation()
					).getValue()) < PI/2)
				{
					// Get border's points
					Vector2D a(actor->getLocation() + hull->borders[j].getA());
					Vector2D b(actor->getLocation() + hull->borders[j].getB());

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
	for (auto const &actor : this->browsableWorld->allActors)
	{
		// if actor - static
		if (actor->getType() != ActorType::Light && actor->getType() != ActorType::Special)
		{
			// get actors geometry
			const Hull *hull = actor->getGeometry();
			// for each border of actor's geometry
			for (int j = 0; j < (int) hull->borders.size(); j++)
			{
				// draw border
				Vector2D A(actor->getLocation() + hull->borders[j].getA()), B(actor->getLocation() + hull->borders[j].getB());
				A = this->project(A);
				B = this->project(B);
				
				this->hge->Gfx_RenderLine(A.x, A.y, B.x, B.y, 0xFFFF00FF);

				// draw normal
				Vector2D LinePos(actor->getLocation() + (hull->borders[j].getA() + hull->borders[j].getB()) / 2);
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
	for (auto const &actor : this->browsableWorld->allActors)
	{
		// if actor - static
		if (actor->getType() == ActorType::Light)
		{
			Vector2D lightLocation(this->project(actor->getLocation()));
			this->hge->Gfx_RenderLine(lightLocation.x - 5, lightLocation.y, lightLocation.x + 5, lightLocation.y, 0xFFAA6600, 0);
			this->hge->Gfx_RenderLine(lightLocation.x, lightLocation.y - 5, lightLocation.x, lightLocation.y + 5, 0xFFAA6600, 0);
		}
	}
}

void Camera::renderPaths()
{
	// for each actors in the world
	for (auto const &point1 : this->browsableWorld->navigationMap)
	{
		Vector2D pathPointLocation(this->project(point1->location));
		
		// render green diamond
		this->hge->Gfx_RenderLine(pathPointLocation.x - 5, pathPointLocation.y, pathPointLocation.x, pathPointLocation.y - 5, 0xFF00FF00, 0);
		this->hge->Gfx_RenderLine(pathPointLocation.x, pathPointLocation.y - 5, pathPointLocation.x + 5, pathPointLocation.y, 0xFF00FF00, 0);
		this->hge->Gfx_RenderLine(pathPointLocation.x + 5, pathPointLocation.y, pathPointLocation.x, pathPointLocation.y + 5, 0xFF00FF00, 0);
		this->hge->Gfx_RenderLine(pathPointLocation.x, pathPointLocation.y + 5, pathPointLocation.x - 5, pathPointLocation.y, 0xFF00FF00, 0);

		// render path arrow
		for (auto const &point2 : point1->legalPoints)
		{
			Vector2D nextPointLocation(this->project(point2->location));

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

void Camera::setLocation(const Vector2D &newLocation)
{
	this->location = newLocation;
}

Vector2D Camera::getResolution() const
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

HTEXTURE Camera::getRenderTexture() const
{
	return this->renderTarget;
}
