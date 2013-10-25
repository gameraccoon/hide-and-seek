#include "Actor.h"


Actor::Actor(World *ownerWorld, HGE *hge, Vector2D location) : Location(location), ColideBox(location, location), Direction(0.0f)
{
	Hge = hge;
	OwnerWorld = ownerWorld;
	OwnerWorld->Spawn(this);
	Type = AT_Ghost;
}

Actor::~Actor(void)
{
	OwnerWorld->Delete(this);
}

Vector2D Actor::GetLocation()
{
	return Location;
}

void Actor::SetLocation(Vector2D newLocation)
{
	Location = newLocation;
}

EActorType Actor::GetType()
{
	return Type;
}

BoundingBox Actor::GetBoundingBox()
{
	return ColideBox;
}

Hull* Actor::GetHull()
{
	return &Geometry;
}