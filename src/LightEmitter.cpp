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
	const bool registered = ActorFactory::RegisterActor(LIGHT_EMITTER_ID, CreateLightEmitter);
}

LightEmitter::LightEmitter(World *ownerWorld, Vector2D location) : Actor(ownerWorld, location, Rotator(0.f))
{
	Type = AT_Light;
	Brightness = 1.f;
	Color = 0xFFFFFFFF;

	ClassID = LIGHT_EMITTER_ID;
}


LightEmitter::~LightEmitter(void)
{
}

void LightEmitter::Update(float deltatime)
{
	Actor::Update(deltatime);
}

void LightEmitter::Render(Vector2D shift, Rotator angle)
{
}

void LightEmitter::UpdateCollision()
{
}

void LightEmitter::TakeDamage(float damageValue,Vector2D impulse)
{
}