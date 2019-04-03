#include "Graphics/Camera.h"

#include <Base/ResourceManager.h>
#include <Components/TransformComponent.h>
#include <Components/CollisionComponent.h>

#include "Base/Engine.h"

Camera::Camera(const World* world, Vector2D resolution, Vector2D location)
	: mBrowsableWorld(world)
	, mLocation(location)
	, mResolution(resolution)
	, mCenterPos(resolution / 2)
	, mAngle(0)
	, mShownSize(512.0f)
	, mFogWidth(512.0f)
{
	mFogScale = mShownSize * 0.9f / (mFogWidth / 2.0f);
	/*
	mRenderTarget = mEngine->Target_Create((int) mResolution.x, (int) mResolution.y, false);
	zone = engine->Target_Create((int) shownSize, (int) shownSize, false);

	collisionSprite = GraphicLoader::Instance().getSprite("collision");
	collisionSprite->SetColor(0xFF00FF00);

	fogSprite = GraphicLoader::Instance().getSprite("fog");
	fogSprite->SetTextureRect(0.0f, 0.0f, fogWidth, fogWidth);
	fogSprite->SetColor(0xFF000000);
	fogSprite->SetHotSpot(fogWidth/2, fogWidth/2);*/

	// Standart parameters of rendering
	mIsShowAABB = false;
	mIsRenderFog = true;
	mIsShowBorders = false;
	mIsRenderShadows = true;
	mIsShowLights = false;
	mIsShowPaths = false;
}

Camera::~Camera()
{
	/*
	engine->Target_Free(renderTarget);
	engine->Target_Free(zone);
	*/
}

void Camera::render()
{
	renderActors(nullptr);
	/*
	// clean lights cached map
	engine->Gfx_BeginScene(renderTarget);
	engine->Gfx_Clear(0);
	engine->Gfx_EndScene();

	// for each light on the scene
	for (auto const &actor : browsableWorld->allActors)
	{
		if (actor->getType() == ActorType::Light)
		{		
			LightEmitter *light = dynamic_cast<LightEmitter*>(actor);
			if (light == nullptr)
				continue;

			engine->Gfx_BeginScene(zone); // start render to texture
			engine->Gfx_Clear(0xFFFFFF);
			
			// actors beside this light
			renderActors(light);

			// shadows for this light
			if (bRenderShadows)
			{
				renderLightShadows(light);
			}

			// fog of this light
			renderFog(shownSize, shownSize, shownSize / fogWidth * light->getBrightness());

			engine->Gfx_EndScene(); // end render to texture

			// prepare the sprite
			engineSprite *lightSprite = new engineSprite(engine->Target_GetTexture(zone), 0, 0, shownSize, shownSize);
			lightSprite->SetBlendMode(BLEND_COLORMUL);
			lightSprite->SetColor(0xFF777777);

			Vector2D projected = project(actor->getLocation());

			// render sprite to frame texture
			engine->Gfx_BeginScene(renderTarget);
			lightSprite->Render(projected.x-shownSize/2, projected.y - shownSize/2);
			engine->Gfx_EndScene();

			delete lightSprite;
		}
	}

	// start rendering to target
	engine->Gfx_BeginScene(renderTarget);

	// Shadows for player's view
	if (bRenderShadows)
	{
		//renderShadows();
	}
*/
	// Render player's fog
	if (mIsRenderFog)
	{
		renderFog(mResolution.x, mResolution.y, mFogScale);
	}

	// Bounding boxes
	if (mIsShowAABB)
	{
		renderCollisionBoxes();
	}
	/*
	// Borders
	if (bShowBorders)
	{
		renderHulls();
	}
	
	// Lights
	if (bShowLights)
	{
		renderLights();
	}

	// Paths
	if (bShowPaths)
	{
		renderPaths();
	}

	// end rendering to target
	engine->Gfx_EndScene();
	*/
}

void Camera::renderActors(const LightComponent* /*light*/)
{
//	const Vector2D lightPos = light->getLocation();
//	const float lightBrightness = light->getBrightness();
	// for each actors in the world
//	for (const auto& actorToRender : mBrowsableWorld->getAllActors())
	{
		// Get actor's camera local location
//		Vector2D screenLoc(actorToRender->getLocation() - lightPos);
		// Get distance from center of screen
//		float distanceFromCamera = screenLoc.size();

		// if actor is not far of drawing zone
		//if (distanceFromCamera >= mShownSize * lightBrightness)
		//	continue;

		// Get sprite
//		auto sprite = mBrowsableWorld->getSprite(actorToRender->getClassID());

//		if (sprite == nullptr)
//		{
//			continue;
//		}

		// calc actor screen location
//		Vector2D newScreenLoc(Vector2D(screenLoc.rotation() + mAngle) * distanceFromCamera);
		// get scaled localtion
//		Vector2D scaledLocation(newScreenLoc + Vector2D(mShownSize/2, mShownSize/2));

		// render actor
		//sprite->RenderEx(scaledLocation.x, scaledLocation.y,
		//	(actorToRender->getRotation() + angle).getValue(),
		//	actorToRender->getScale().x, actorToRender->getScale().y);
//		auto loc = actorToRender->getLocation();
	}
}

void Camera::renderCollisionBoxes()
{
	/*
	// for each actors in the world
	for (const auto& collisions : mBrowsableWorld->getComponents<CollisionComponent>())
	{
		// get location and size of actor's AABB (axis-aligned bounding box)
		Vector2D min = collisions->getBoundingBox().getFirst();
		Vector2D size = collisions->getBoundingBox().getThird() - min;

		// project location on the screen
		Vector2D shift = project(min);

		// render collision sprite
		//mCollisionTexture->RenderEx(shift.x, shift.y, angle, size.x/64.0f, size.y/64.0f);
	}*/
}

Vector2D Camera::project(const Vector2D &worldPoint) const
{
	// calc camera-location
	Vector2D screenLoc(worldPoint - mLocation);
	// calc screen-coordinates relativety camera center
	Vector2D newScreenLoc(Vector2D(screenLoc.rotation() + Rotator(mAngle)) * screenLoc.size());
	// calc absolute screen-coordinates
	return mCenterPos + newScreenLoc;
}

Vector2D Camera::projectFrom(const Vector2D &worldPoint, const Vector2D &projectionCenter) const
{
	// calc camera-location
	Vector2D screenLoc(worldPoint - projectionCenter);
	// calc screen-coordinates relativety camera center
	Vector2D newScreenLoc(Vector2D(screenLoc.rotation() + Rotator(mAngle)) * screenLoc.size());
	// calc absolute screen-coordinates
	return Vector2D(mShownSize/2, mShownSize/2) + newScreenLoc;
}

Vector2D Camera::deProject(const Vector2D &screenPoint) const
{
	// calc relative screen-coordinates
	Vector2D relScreenLoc(screenPoint - mCenterPos);
	// calc world-coordinates relativety camera
	Vector2D screenLoc = Vector2D(relScreenLoc.rotation() - Rotator(mAngle)) * relScreenLoc.size();
	// calc world location
	return mLocation + screenLoc;
}

void Camera::renderFog(float width, float height, float size)
{
	// render fog sprite
	//mFogTexture->RenderEx(width/2, height/2, 0, size, size);

	// left shadow quad
	drawQuad(Vector2D(-2000.f, -2000.f), Vector2D(-2000.f, 2000.f),
		Vector2D(width / 2 - size * mFogWidth / 2, 2000.f),
		Vector2D(width / 2 - size * mFogWidth / 2, -2000.f));

	// right shadow quad
	drawQuad(Vector2D(2000.f, -2000.f), Vector2D(2000.f, 2000.f),
		Vector2D(width/2 + size * mFogWidth / 2, 2000.f),
		Vector2D(width/2 + size * mFogWidth / 2, -2000.f));

	// up shadow quad
	drawQuad(Vector2D(-2000.f, -2000.f), Vector2D(2000.f, -2000.f),
		Vector2D(2000.f, height/2 - size * mFogWidth/2),
		Vector2D(-2000.f, height/2 - size * mFogWidth/2));

	// down shadow quad
	drawQuad(Vector2D(-2000.f, 2000.f), Vector2D(2000.f, 2000.f),
		Vector2D(2000.f, height/2 + size * mFogWidth/2),
		Vector2D(-2000.f, height/2 + size * mFogWidth/2));
}

void Camera::drawPenumbra(const Vector2D &, const Vector2D &, const Vector2D &)
{
	/*HTEXTURE PenumbraTexture = GraphicLoader::Instance().getTexture("penumbra.png");
	
	engineTriple triple;
	//triple.
	triple.tex = PenumbraTexture;
	triple.blend = BLEND_DEFAULT;
	triple.v[0].z = triple.v[1].z = triple.v[2].z = 0;
	triple.v[0].x = first.x;
	triple.v[0].y = first.y;
	triple.v[1].x = second.x;
	triple.v[1].y = second.y;
	triple.v[2].x = third.x;
	triple.v[2].y = third.y;
	
	triple.v[0].col = 0xFFFFFFFF;
	triple.v[0].tx = 1;
	triple.v[0].ty = 1;
	
	triple.v[1].col = 0xFFFFFFFF;
	triple.v[1].tx = 0;
	triple.v[1].ty = 1;
	
	triple.v[2].col = 0xFFFFFFFF;
	triple.v[2].tx = 1;
	triple.v[2].ty = 0;

	engine->Gfx_RenderTriple(&triple);*/
}

void Camera::drawQuad(const Vector2D &, const Vector2D &, const Vector2D &, const Vector2D &)
{
	/*engineQuad quad;
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

	engine->Gfx_RenderQuad(&quad);*/
}

void Camera::renderLightShadows(const LightComponent* /*light*/)
{
//	auto movementWeak = light->getTransformComponent();
//	auto movement = movementWeak.lock();
//	if (movement == nullptr)
//	{
//		return;
//	}

//	const Vector2D lightPos = movement->getLocation();
//	const float lightBrightness = light->getBrightness();

//	Vector2D lightCenterPos(mShownSize/2, mShownSize/2);
//	// for each actors in the world
//	for (const auto& actor : mBrowsableWorld->getAllActors())
//	{
//		// if actor - static
//		if (actor->getType() == ActorType::Static)
//		{
//			auto actorTransformComponent = actor->getSingleComponent<TransformComponent>();
//			if (actorTransformComponent == nullptr)
//			{
//				continue;
//			}

//			auto actorCollisionComponent = actor->getSingleComponent<CollisionComponent>();
//			if (actorCollisionComponent == nullptr)
//			{
//				continue;
//			}

//			Vector2D actorLocation = actorTransformComponent->getLocation();

//			// Get actor's camera local location
//			Vector2D screenLoc(actorLocation - lightPos);
//			// Get distance from center of screen
//			float distanceFromCamera = screenLoc.size();

//			// if actor is not far of drawing zone
//			if (distanceFromCamera >= mShownSize * lightBrightness)
//				continue;

//			// get actors geometry
//			const Hull *hull = actorCollisionComponent->getGeometry();
//			// for each border of actor's geometry
//			for (size_t j = 0; j < hull->borders.size(); j++)
//			{
//				// if border's normal and camera view on border have different directions (angle > PI/2)
//				if (abs((
//						hull->borders[j].getNormal().rotation()
//						- (actorLocation + (hull->borders[j].getA() + hull->borders[j].getB())/2 - lightPos).rotation()
//					).getValue()) < PI/2)
//				{
//					// Get border's points
//					Vector2D a(actorLocation + hull->borders[j].getA());
//					Vector2D b(actorLocation + hull->borders[j].getB());

//					// project them on screen
//					a = projectFrom(a, lightPos);
//					b = projectFrom(b, lightPos);

//					// Draw shadow's quad of this border to far of the screen
//					drawQuad(a, b, b + (b - lightCenterPos).ort() * 3000, a + (a - lightCenterPos).ort() * 3000);
//				}
//			}
//		}
//	}
}

void Camera::renderShadows()
{
//	// for each actors in the world
//	for (const auto& actor : mBrowsableWorld->getAllActors())
//	{
//		// if actor - static
//		if (actor->getType() == ActorType::Static)
//		{
//			auto actorTransformComponent= actor->getSingleComponent<TransformComponent>();
//			if (actorTransformComponent == nullptr)
//			{
//				continue;
//			}

//			auto actorCollisionComponent = actor->getSingleComponent<CollisionComponent>();
//			if (actorCollisionComponent == nullptr)
//			{
//				continue;
//			}

//			Vector2D actorLocation = actorTransformComponent->getLocation();

//			// get actors geometry
//			const Hull *hull = actorCollisionComponent->getGeometry();
//			// for each border of actor's geometry
//			for (size_t j = 0; j < hull->borders.size(); j++)
//			{
//				// if border's normal and camera view on border have different directions (angle > PI/2)
//				if (abs((
//						hull->borders[j].getNormal().rotation()
//						- (actorLocation + (hull->borders[j].getA() + hull->borders[j].getB())/2 - mLocation).rotation()
//					).getValue()) < PI/2)
//				{
//					// Get border's points
//					Vector2D a(actorLocation + hull->borders[j].getA());
//					Vector2D b(actorLocation + hull->borders[j].getB());

//					// project them on screen
//					a = project(a);
//					b = project(b);

//					// Draw shadow's quad of this border to far of the screen
//					drawQuad(a, b, b + (b - mCenterPos).ort() * 3000, a + (a - mCenterPos).ort() * 3000);
//				}
//			}
//		}
//	}
}

void Camera::renderHulls()
{
//	const int normal_length = 10;

//	// for each actors in the world
//	for (const auto& actor : mBrowsableWorld->getAllActors())
//	{
//		// if actor - static
//		if (actor->getType() != ActorType::Light && actor->getType() != ActorType::Special)
//		{
//			auto actorTransformComponent = actor->getSingleComponent<TransformComponent>();
//			if (actorTransformComponent == nullptr)
//			{
//				continue;
//			}

//			auto actorCollisionComponent = actor->getSingleComponent<CollisionComponent>();
//			if (actorCollisionComponent == nullptr)
//			{
//				continue;
//			}

//			Vector2D actorLocation = actorTransformComponent->getLocation();

//			// get actors geometry
//			const Hull *hull = actorCollisionComponent->getGeometry();
//			// for each border of actor's geometry
//			for (size_t j = 0; j < hull->borders.size(); j++)
//			{
//				// draw border
//				Vector2D A(actorLocation + hull->borders[j].getA()), B(actorLocation + hull->borders[j].getB());
//				A = project(A);
//				B = project(B);
				
//				//engine->Gfx_RenderLine(A.x, A.y, B.x, B.y, 0xFFFF00FF);

//				// draw normal
//				Vector2D LinePos(actorLocation + (hull->borders[j].getA() + hull->borders[j].getB()) / 2);
//				Vector2D Norm(LinePos + hull->borders[j].getNormal() * static_cast<float>(normal_length));

//				LinePos = project(LinePos);
//				Norm = project(Norm);

//				//engine->Gfx_RenderLine(LinePos.x, LinePos.y, Norm.x, Norm.y, 0xFF0000FF);
//			}
//		}
//	}
}

void Camera::renderLights()
{
//	// for each actors in the world
//	for (const auto& actor : mBrowsableWorld->getAllActors())
//	{
//		// if actor - static
//		if (actor->getType() == ActorType::Light)
//		{
////			Vector2D lightLocation(project(actor->getLocation()));
//			//engine->Gfx_RenderLine(lightLocation.x - 5, lightLocation.y, lightLocation.x + 5, lightLocation.y, 0xFFAA6600, 0);
//			//engine->Gfx_RenderLine(lightLocation.x, lightLocation.y - 5, lightLocation.x, lightLocation.y + 5, 0xFFAA6600, 0);
//		}
//	}
}

void Camera::renderPaths()
{
	/*
	// for each actors in the world
	for (auto const &point1 : browsableWorld->navigationMap)
	{
		Vector2D pathPointLocation(project(point1->location));
		
		// render green diamond
		engine->Gfx_RenderLine(pathPointLocation.x - 5, pathPointLocation.y, pathPointLocation.x, pathPointLocation.y - 5, 0xFF00FF00, 0);
		engine->Gfx_RenderLine(pathPointLocation.x, pathPointLocation.y - 5, pathPointLocation.x + 5, pathPointLocation.y, 0xFF00FF00, 0);
		engine->Gfx_RenderLine(pathPointLocation.x + 5, pathPointLocation.y, pathPointLocation.x, pathPointLocation.y + 5, 0xFF00FF00, 0);
		engine->Gfx_RenderLine(pathPointLocation.x, pathPointLocation.y + 5, pathPointLocation.x - 5, pathPointLocation.y, 0xFF00FF00, 0);

		// render path arrow
		for (auto const &point2 : point1->legalPoints)
		{
			Vector2D nextPointLocation(project(point2->location));

			engine->Gfx_RenderLine(pathPointLocation.x, pathPointLocation.y, nextPointLocation.x, nextPointLocation.y, 0xFF00FF00, 0);
			Vector2D thirdPoint = pathPointLocation + (nextPointLocation - pathPointLocation)/3;

			Vector2D Direction = (nextPointLocation - pathPointLocation).ort();
			float arrowlength = 5.f;
			
			engine->Gfx_RenderLine(thirdPoint.x,
				thirdPoint.y,
				thirdPoint.x + (Direction.normal().x - Direction.ort().x) * arrowlength,
				thirdPoint.y + (Direction.normal().y - Direction.ort().y) * arrowlength, 0xFF00FF00, 0);
			
			engine->Gfx_RenderLine(thirdPoint.x,
				thirdPoint.y,
				thirdPoint.x - (Direction.normal().x + Direction.ort().x) * arrowlength,
				thirdPoint.y - (Direction.normal().y + Direction.ort().y) * arrowlength, 0xFF00FF00, 0);
		}
	}
	*/
}

void Camera::setLocation(const Vector2D &newLocation)
{
	mLocation = newLocation;
}

Vector2D Camera::getResolution() const
{
	return mResolution;
}

void Camera::setRotation(Rotator angle)
{
	angle = Rotator(angle.getValue());
}

void Camera::showAABB(bool show)
{
	mIsShowAABB = show;
}

void Camera::showFog(bool show)
{
	mIsRenderFog = show;
}

void Camera::showShadows(bool show)
{
	mIsRenderShadows = show;
}

void Camera::showHulls(bool show)
{
	mIsShowBorders = show;
}

void Camera::showLights(bool show)
{
	mIsShowLights = show;
}

void Camera::showPaths(bool show)
{
	mIsShowPaths = show;
}

/*HTEXTURE Camera::getRenderTexture() const
{
	return renderTarget;
}
*/
