#include "Hero.h"


Hero::Hero(void)
{
	Location = Vector2D(0.0f, 0.0f);
}


Hero::~Hero(void)
{
}

void Hero::SetLocation(Vector2D location)
{
	Location = location;
}

void Hero::MoveTo(Vector2D newLocation)
{
	NextLocation = newLocation;
}

void Hero::Process(int deltatime)
{
//	Vector deltaLocation = NextLocation - Location;
//	Location.Add(Speed * deltatime * deltaLocation / deltaLocation.Size());
}