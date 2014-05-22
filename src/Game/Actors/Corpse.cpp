#include "Corpse.h"


#include <Modules/ActorFactory.h>
// unnamed namespase to hide from another places
namespace
{
	// specific factory
	IActor* CreateCorpse(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new Corpse(world, location, scale, rotation);
	}

	const std::string CLASS_ID = "Corpse";

	// register specific factory in actor factory
	const bool registered = ActorFactory::Factory().registerActor(CLASS_ID, CreateCorpse);
}


Corpse::Corpse(World *world, Vector2D location, Vector2D scale, Rotator rotation) :
	Actor(world, location, rotation)
{
	this->updateActorId(CLASS_ID);
}


Corpse::~Corpse(void)
{
}

void Corpse::updateCollision()
{
}