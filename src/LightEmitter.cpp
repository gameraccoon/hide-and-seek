#include "LightEmitter.h"


LightEmitter::LightEmitter(World *ownerWorld, Vector2D location) : Actor(ownerWorld, location)
{
	Type = AT_Special;
	Brightness = 1.f;
	Color = 0xFFFFFFFF;
}


LightEmitter::~LightEmitter(void)
{
}
