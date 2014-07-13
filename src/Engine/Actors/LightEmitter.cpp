#include "LightEmitter.h"

LightEmitter::LightEmitter(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Actor(world, location, rotation)
{
	this->setType(ActorType::Light);
	this->brightness = 1.0f;
	this->color = 0xFFFFFFFF;

	this->setOriginalSize(Vector2D(10, 10));
	Hull geometry;
	geometry.points.insert(geometry.points.end(), -this->getOriginalSize() / 2);
	geometry.points.insert(geometry.points.end(), (this->getOriginalSize() / 2).mirrorV());
	geometry.points.insert(geometry.points.end(), this->getOriginalSize() / 2);
	geometry.points.insert(geometry.points.end(), (this->getOriginalSize() / 2).mirrorH());
	geometry.generate();
	this->setGeometry(geometry);
	this->updateGeometry();

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