#include "LightEmitter.h"

#include "../../Engine/Modules/ActorFactory.h"
// unnamed namespase to hide from another places
namespace
{
	// specific factory
	IActor* CreateLightEmitter(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new LightEmitter(world, location, scale, rotation);
	}

	const std::string CLASS_ID = "LightEmitter";

	// register specific factory in actor factory
	const bool registered = ActorFactory::Factory().registerActor(CLASS_ID, CreateLightEmitter);
}


LightEmitter::LightEmitter(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Actor(world, location, rotation)
{
	this->type = ActorType::Light;
	this->brightness = 1.f;
	this->color = 0xFFFFFFFF;

	this->updateActorId(CLASS_ID);
}


LightEmitter::~LightEmitter(void)
{
}

void LightEmitter::update(float deltatime)
{
	Actor::update(deltatime);
}

void LightEmitter::updateCollision()
{
}

void LightEmitter::takeDamage(float damageValue,Vector2D impulse)
{
}
