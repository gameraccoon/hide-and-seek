#include "LightEmitter.h"

LightEmitter::LightEmitter(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Actor(world, location, rotation)
{
	this->setType(ActorType::Light);
	this->brightness = 1.f;
	this->color = 0xFFFFFFFF;

	this->updateActorId("LightEmitter");
}


LightEmitter::~LightEmitter(void)
{
}

void LightEmitter::updateCollision()
{
}