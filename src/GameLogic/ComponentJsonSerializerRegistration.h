#pragma once

#include "ECS/Serialization/JsonComponentSerializer.h"

namespace ComponentsRegistration
{
	void RegisterJsonSerializers(JsonComponentSerializationHolder& jsonSerializerHolder);
}
