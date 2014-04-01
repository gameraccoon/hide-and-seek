#include "Corpse.h"


// ## This is automatic generated text. Pleace do not change it.
// ## Registration in ActorFactory
#include "../../Engine/Modules/ActorFactory.h"
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
// ## End of automatic generated text


Corpse::Corpse(World *world, Vector2D location, Vector2D scale, Rotator rotation) :
	Actor(world, location, rotation)
{
	this->classID = CLASS_ID;
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