#include "ActorFactory.h"

#include <Debug/Log.h>

ActorFactory::ActorFactory() {}
ActorFactory::~ActorFactory()
{
	Log::Instance().writeLog("ActorFactory destroyed");
}

ActorFactory& ActorFactory::Factory()
{
	static ActorFactory singleInstance;
	return singleInstance;
}

bool ActorFactory::registerActor(std::string actorId, CreateActorCallback createFn)
{
	return this->callbacks.insert(CallbackMap::value_type(actorId, createFn)).second;
}

bool ActorFactory::unregisterActor(std::string actorId)
{
	return this->callbacks.erase(actorId) == 1;
}

IActor* ActorFactory::placeActor(std::string actorId, World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	CallbackMap::const_iterator it = this->callbacks.find(actorId);
	
	if (it == this->callbacks.end())
	{
		throw std::runtime_error(std::string("Unknown actor identefier '").append(actorId).append("'"));
	}
	
	return (it->second)(world, location, scale, rotation);
}