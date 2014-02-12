#include "ActorFactory.h"

ActorFactory::CallbackMap ActorFactory::Callbacks = CallbackMap();

bool ActorFactory::RegisterActor(std::string actorId, CreateActorCallback createFn)
{
	return Callbacks.insert(CallbackMap::value_type(actorId, createFn)).second;
}

bool ActorFactory::UnregisterActor(std::string actorID)
{
	return Callbacks.erase(actorID) == 1;
}

IActor* ActorFactory::CreateActor(std::string actorID, World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
{
	CallbackMap::const_iterator it = Callbacks.find(actorID);
	
	if (it == Callbacks.end())
	{
		throw std::runtime_error("Unknown actor identefier");
	}

	return (it->second)(world, location, scale, rotation);
}