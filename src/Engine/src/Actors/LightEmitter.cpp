#include "Actors/LightEmitter.h"

LightEmitter::LightEmitter(World *world, Vector2D location, Vector2D /*scale*/, Rotator rotation) : Actor(world, location, rotation)
{
	setType(ActorType::Light);
	mBrightness = 1.0f;
	mColor = 0xFFFFFFFF;

	setOriginalSize(Vector2D(10.0f, 10.0f));
	Hull geometry;
	geometry.type = Hull::Type::Circular;
	geometry.setRadius(10.0f);
	setGeometry(geometry);

	updateActorId("LightEmitter");
}


LightEmitter::~LightEmitter()
{
}

float LightEmitter::getBrightness() const
{
	return mBrightness;
}

void LightEmitter::setBrightness(float brightness)
{
	mBrightness = brightness;
}
