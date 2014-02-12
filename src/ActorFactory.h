#ifndef ACTOR_FACTORY
#define ACTOR_FACTORY

#include <map>

#include "../src/Globals.h"

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
	#define RELEASE
	#define WARN(message)
	#define WARN_IF(condition, message)
#endif

#include "../src/IActor.h"
#include "../src/Vector2D.h"
#include "../src/Rotator.h"
#include "../src/World.h"

/**
 * Factory that allows to create any type of registered actor by their name identifier.
 *
 * Based on pattern Factory in book Andrei Alexandrescu "Modern C++ Design: Generic Programming and Design Patterns Applied"
 */
class ActorFactory
{
public:
	typedef IActor* (*CreateActorCallback)(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation);
private:
	typedef std::map<std::string, CreateActorCallback> CallbackMap;
public:
	/**
	 * register new actor class by identifier and "create" function
	 * @return true if class was not registered earlier
	 */
	static bool RegisterActor(std::string actorId, CreateActorCallback createFn);

	/**
	 * unregister already registred class
	 * @return true if class was unregistered
	 */ 
	static bool UnregisterActor(std::string actorID);

	/**
	 * Create actor by identifier.
	 */
	static IActor* CreateActor(std::string actorID, World *world, const Vector2D location, const Vector2D size, const Rotator rotation);
private:
	static CallbackMap Callbacks;
};

#endif