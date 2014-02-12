#include "Actor.h"

Actor::Actor(World *ownerWorld, Vector2D location, Rotator rotation) : Location(location),
																	ColideBox(location, location),
																	Direction(rotation),
																	Scale(0.f, 0.f),
																	OriginalSize(1.f, 1.f),
																	CalculatedSize(1.f, 1.f)
{
	Hge = ownerWorld->GetHge();
	OwnerWorld = ownerWorld;
	OwnerWorld->Spawn(this);
	Type = AT_Ghost;

	ClassID = "Actor";
}

Actor::~Actor(void)
{
	OwnerWorld->Delete(this);
}

void Actor::SetLocation(const Vector2D& newLocation)
{
	Location = Vector2D(newLocation);
}

Vector2D Actor::GetLocation()
{
	return Location;
}

void Actor::SetRotation(const Rotator& newRotation)
{
	Direction = Rotator(newRotation);
}

Rotator Actor::GetRotation()
{
	return Direction;
}

void Actor::SetScale(const Vector2D& newScale)
{
	Scale = Vector2D(newScale);
	CalculatedSize.X = OriginalSize.X * Scale.X;
	CalculatedSize.Y = OriginalSize.Y * Scale.Y;
}

Vector2D Actor::GetScale()
{
	return Scale;
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

std::string Actor::GetClassID()
{
	return ClassID;
}

void Actor::Update(float deltaTime)
{
}

void Actor::Render(Vector2D shift, Rotator angle)
{
}