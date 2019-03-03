#include "Actors/LightEmitter.h"

#include <Components/MovementComponent.h>
#include <Components/LightComponent.h>

LightEmitter::LightEmitter(World *world, Vector2D location, Rotator rotation)
	: Actor(world)
{
	auto movement = makeAndAddComponent<MovementComponent>(location, rotation);
	auto light = std::make_shared<LightComponent>();
	light->setMovementComponent(movement);
	addComponent(light);

	setType(ActorType::Light);
	mBrightness = 1.0f;
	mColor = 0xFFFFFFFF;

//	setOriginalSize(Vector2D(10.0f, 10.0f));
//	Hull geometry;
//	geometry.type = Hull::Type::Circular;
//	geometry.setRadius(10.0f);
//	setGeometry(geometry);

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
