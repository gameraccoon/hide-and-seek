#include "FloatingCamera.h"


FloatingCamera::FloatingCamera(Engine::Internal::Engine* engine, const World* world, Vector2D resolution, Vector2D location)
	: Camera(engine, world, resolution, location)
	, mShift(ZERO_VECTOR)
{
}


FloatingCamera::~FloatingCamera()
{
}

void FloatingCamera::setLocation(const Vector2D &newLocation)
{
	mLocation = newLocation;
	mCenterPos = mResolution / 2 - mShift;
}

void FloatingCamera::setCenterShift(Vector2D shift)
{
	shift = shift;
	mCenterPos = mResolution/2 - shift;
}

void FloatingCamera::renderFog()
{
	//mFogSprite->RenderEx(mResolution.x/2 - mShift.x, mResolution.y/2 - mShift.y, 0, mFogScale, mFogScale);
	//Camera::RenderFog();
}
