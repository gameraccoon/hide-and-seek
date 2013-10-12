#include "Hero.h"


Hero::Hero(void)
{
}


Hero::~Hero(void)
{
}

void Hero::SetLocation(Vector location)
{
	Location = location;
}

void Hero::MoveTo(Vector newLocation)
{
	NextLocation = newLocation;
}

void Hero::Process(int deltatime)
{
//	Vector deltaLocation = NextLocation - Location;
//	Location.Add(Speed * deltatime * deltaLocation / deltaLocation.Size());
}