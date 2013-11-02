#include "Weapon.h"


Weapon::Weapon()
{
}


Weapon::~Weapon(void)
{
}

void Weapon::StartShooting(Vector2D currentLocation, Vector2D targetLocation)
{
	OwnerWorld->Spawn(new Bullet(OwnerWorld, currentLocation, targetLocation));
}

void Weapon::StopShooting()
{
	
}

void Weapon::ChangeDirection(Vector2D targetLocation)
{

}

void Weapon::SetOwnerWorld(World *world)
{
	OwnerWorld = world;
}