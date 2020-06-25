#pragma once

#include "ECS/ComponentFactory.h"
#include "ECS/Serialization/JsonComponentSerializer.h"

// Aggregates everything needed to construct, serialize or deserialize a component
struct ComponentSerializersHolder
{
	ComponentFactory factory;
	JsonComponentSerializationHolder jsonSerializer;

	ComponentSerializersHolder() = default;
	ComponentSerializersHolder(ComponentSerializersHolder&) = delete;
	ComponentSerializersHolder& operator=(ComponentSerializersHolder&) = delete;
};
