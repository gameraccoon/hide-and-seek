#include "LightEmitter.h"


LightEmitter::LightEmitter(World *ownerWorld, Vector2D location) : Actor(ownerWorld, location)
{
	Type = AT_Light;
	Brightness = 1.f;
	Color = 0xFFFFFFFF;
}


LightEmitter::~LightEmitter(void)
{
}

void LightEmitter::Update(float deltaTime)
{
}

void LightEmitter::Render(Vector2D shift, Rotator angle)
{
}

void LightEmitter::UpdateCollision()
{
}

void LightEmitter::TakeDamage(float damageValue,Vector2D impulse)
{
}