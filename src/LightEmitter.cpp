#include "LightEmitter.h"

#include "../src/ActorFactory.h"

// unnamed namespase to hide from another places
namespace
{
	// specific factory
	IActor* CreateLightEmitter(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new LightEmitter(world, location);
	}

	const std::string LIGHT_EMITTER_ID = "LightEmitter";

	// register specific factory in actor factory
	const bool registered = ActorFactory::registerActor(LIGHT_EMITTER_ID, CreateLightEmitter);
}

LightEmitter::LightEmitter(World *ownerWorld, Vector2D location) : Actor(ownerWorld, location, Rotator(0.f))
{
	this->type = AT_Light;
	this->brightness = 1.f;
	this->color = 0xFFFFFFFF;

	this->classID = LIGHT_EMITTER_ID;
}


LightEmitter::~LightEmitter(void)
{
}

void LightEmitter::update(float deltatime)
{
	Actor::update(deltatime);
}

void LightEmitter::render(Vector2D shift, Rotator angle)
{
}

void LightEmitter::updateCollision()
{
}

void LightEmitter::takeDamage(float damageValue,Vector2D impulse)
{
}
