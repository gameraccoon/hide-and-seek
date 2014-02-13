#include "Weapon.h"


Weapon::Weapon()
{
}


Weapon::~Weapon(void)
{
}

void Weapon::startShooting(Vector2D currentLocation, Vector2D targetLocation)
{
	this->ownerWorld->spawnActor(new Bullet(this->ownerWorld, currentLocation, targetLocation));
}

void Weapon::stopShooting()
{
	
}

void Weapon::changeDirection(Vector2D targetLocation)
{

}

void Weapon::setOwnerWorld(World *world)
{
	this->ownerWorld = world;
}