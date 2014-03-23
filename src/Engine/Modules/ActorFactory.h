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
	 * register new actor class by an identifier and a "create" function
	 * @return true if class was not registered earlier
	 */
	bool registerActor(std::string actorId, CreateActorCallback createFn);

	/**
	 * unregister already registred class
	 * @return true if class was unregistered
	 */ 
	bool unregisterActor(std::string actorID);

	/**
	 * Create actor by identifier.
	 */
	IActor* createActor(std::string actorID, World *world, const Vector2D location, const Vector2D size, const Rotator rotation);
private:
	CallbackMap callbacks;

	static ActorFactory* singleInstance;

	/*
	 * Turn off unusable operations
	 */
	ActorFactory();
	ActorFactory(const ActorFactory&);
	~ActorFactory();
	ActorFactory& operator=(const ActorFactory&);
};

#endif
