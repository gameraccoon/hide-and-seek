#include "Corpse.h"

#include "../../Engine/Modules/ActorFactory.h"


// unnamed namespase to hide from another places
namespace
{
	// specific factory
	IActor* CreateCorpse(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new Corpse(world, location, rotation);
	}

	const std::string CORPSE_ID = "Corpse";

	// register specific factory in actor factory
	const bool registered = ActorFactory::registerActor(CORPSE_ID, CreateCorpse);
}

Corpse::Corpse(World *ownerWorld, Vector2D location, Rotator rotation) :
	Actor(ownerWorld, location, rotation)
{
	this->classID = CORPSE_ID;
}


Corpse::~Corpse(void)
{
}

void Corpse::takeDamage(float damageValue, Vector2D impulse)
{
}

void Corpse::updateCollision()
{
}