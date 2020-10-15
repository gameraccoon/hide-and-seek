#pragma once

#include <string>

class World;
class ComponentSerializersHolder;

namespace Utils
{
	void SaveWorld(World& world, const std::string& fileName, const ComponentSerializersHolder& serializationHolder);
}
