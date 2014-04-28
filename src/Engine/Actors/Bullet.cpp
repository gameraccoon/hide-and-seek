#include "Bullet.h"


// ## This is automatic generated text. Pleace do not change it.
// ## Registration in ActorFactory
#include "../../Engine/Modules/ActorFactory.h"
// unnamed namespase to hide from another places
namespace
{
	// specific factory
	IActor* CreateBullet(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new Bullet(world, location, scale, rotation);
	}

	const std::string CLASS_ID = "Bullet";

	// register specific factory in actor factory
	const bool registered = ActorFactory::Factory().registerActor(CLASS_ID, CreateBullet);
}
// ## End of automatic generated text


#include "../../Engine/Helpers/DebugMethods.h"

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
 	#define RELEASE
  	#define WARN(message)
  	#define WARN_IF(condition, message)
#endif


Bullet::Bullet(World *world, Vector2D location, Vector2D scale, Rotator rotation) :
	Actor(world, location, rotation)
{
	this->speed = 10.0f;	
	this->type = ActorType::Bullet;

	this->speed = 450.0f;

	this->updateActorId(CLASS_ID);
}

Bullet::~Bullet(void)
{
}

void Bullet::update(float deltatime)
{
	Vector2D newLocation = this->location + deltatime * this->speed * Vector2D(this->direction);
	
	WARN_IF(!this->ownerWorld, "Not assigned OwnerWorld for bullet");

	RayTrace ray(this->ownerWorld, this->location, newLocation);
	Vector2D traceLocation(ZERO_VECTOR);
	IActor *trasedActor = ray.trace(&traceLocation);

	// if there nothing to hit
	if (trasedActor == nullptr)
	{
		this->location = newLocation;
	}
	else // bullet is hiting some actor
	{
		trasedActor->takeDamage(10.f, Vector2D(this->direction) * this->speed * 0.01f);
		this->speed = 0.0f;
		this->destroy();
	}

	// bullet will be destroyed after 10 second
	if (this->lifetime > 10.f)
	{
		this->destroy();
	}

	Actor::update(deltatime);
}

void Bullet::updateCollision()
{
}

void Bullet::takeDamage(float damageValue,Vector2D impulse)
{
	// do nothing for now
}

void Bullet::updateActorId(std::string classId)
{
	this->classID = classId;
	this->actorId = "some" + classId;
}
