#include "Actor.h"


Actor::Actor(Vector2D location) : Location(location)
{
}

Actor::~Actor(void)
{
}

Vector2D Actor::GetLocation()
{
	return Location;
}

void Actor::SetLocation(Vector2D newLocation)
{
	Location = newLocation;
}

void Actor::Render()
{
	Sprite->RenderEx(Location.X, Location.Y, Direction);
}