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
	return this->callbacks.insert(CallbackMap::value_type(actorId, createFn)).second;
}

bool ActorFactory::unregisterActor(std::string actorId)
{
	return this->callbacks.erase(actorId) == 1;
}

IActor* ActorFactory::createActor(std::string actorId, World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	CallbackMap::const_iterator it = this->callbacks.find(actorId);
	
	if (it == this->callbacks.end())
	{
		throw std::runtime_error("Unknown actor identefier");
	}

	return (it->second)(world, location, scale, rotation);
}