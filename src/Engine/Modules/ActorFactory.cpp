#include "ActorFactory.h"

ActorFactory* ActorFactory::singleInstance = NULL;

ActorFactory::ActorFactory() {}
ActorFactory::ActorFactory(const ActorFactory&) {}
ActorFactory::~ActorFactory() {}
ActorFactory& ActorFactory::operator=(const ActorFactory&)
{
	return *ActorFactory::singleInstance;
};

ActorFactory& ActorFactory::Factory()
{
	if (ActorFactory::singleInstance == NULL)
	{
		ActorFactory::singleInstance = new ActorFactory();
	}

	return *ActorFactory::singleInstance;
}

bool ActorFactory::registerActor(std::string actorId, CreateActorCallback createFn)
{
	return callbacks.insert(CallbackMap::value_type(actorId, createFn)).second;
}

bool ActorFactory::unregisterActor(std::string actorID)
{
	return callbacks.erase(actorID) == 1;
}

IActor* ActorFactory::createActor(std::string actorID, World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	CallbackMap::const_iterator it = callbacks.find(actorID);
	
	if (it == callbacks.end())
	{
		throw std::runtime_error("Unknown actor identefier");
	}

	return (it->second)(world, location, scale, rotation);
}