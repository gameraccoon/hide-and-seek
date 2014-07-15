#include "LightEmitter.h"

LightEmitter::LightEmitter(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Actor(world, location, rotation)
{
	this->setType(ActorType::Light);
	this->brightness = 1.0f;
	this->color = 0xFFFFFFFF;

	this->setOriginalSize(Vector2D(10.0f, 10.0f));
	Hull geometry;
	geometry.type = Hull::Type::Circular;
	geometry.setRadius(10.0f);
	this->setGeometry(geometry);

	this->updateActorId("LightEmitter");
}


LightEmitter::~LightEmitter(void)
{
}

float LightEmitter::getBrightness() const
{
	return this->brightness;
}

void LightEmitter::setBrightness(float brightness)
{
	this->brightness = brightness;
}