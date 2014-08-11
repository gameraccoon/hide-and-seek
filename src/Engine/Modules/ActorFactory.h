#ifndef ACTOR_FACTORY
#define ACTOR_FACTORY

#include <map>
#include "../Core/World.h"

/**
 * Factory that allows to create any type of registered actor by their name identifier.
 *
 * Based on pattern Factory in book Andrei Alexandrescu "Modern C++ Design: Generic Programming and Design Patterns Applied"
 *
 * ActorFactory is a singleton.
 */
class ActorFactory
{
public:
	typedef IActor* (*CreateActorCallback)(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation);
private:
	typedef std::map<std::string, CreateActorCallback> CallbackMap;
public:
	/**
	 * @return the single instance of the ActorFactory
	 */
	static ActorFactory& Factory();

	/**
	 * Register new actor class by an identifier and a "create" function.
	 * If identifier already exists, factory will not be registered.
	 *
	 * @return true if class wasn't registered earlier
	 */
	bool registerActor(std::string actorId, CreateActorCallback createFn);

	/**
	 * Unregister already registred class.
	 *
	 * @return true if class was unregistered
	 */ 
	bool unregisterActor(std::string actorId);

	/**
	 * Create actor by identifier.
	 *
	 * @throws runtime_error when actorId is unknown
	 */
	IActor* placeActor(std::string actorId, World *world, const Vector2D location, const Vector2D size, const Rotator rotation);
private:
	CallbackMap callbacks;

	/*
	 * Turn off unusable operations
	 */
	ActorFactory();
	~ActorFactory();
	ActorFactory(const ActorFactory&);
	void operator=(const ActorFactory&);
};

#endif
