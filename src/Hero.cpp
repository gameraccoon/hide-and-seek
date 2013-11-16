#include "Hero.h"

#include <typeinfo.h>

Hero::Hero(World *ownerWorld, Vector2D location) : Actor(ownerWorld, location), Step(ZeroVector), Size(32.0f, 32.0f)
{
	Type = AT_Living;

	UpdateCollision();

	Speed = 12.0f;

	Geometry.Points.insert(Geometry.Points.end(), -Size / 2);
	Geometry.Points.insert(Geometry.Points.end(), (Size / 2).MirrorV());
	Geometry.Points.insert(Geometry.Points.end(), Size / 2);
	Geometry.Points.insert(Geometry.Points.end(), (Size / 2).MirrorH());

	HeroTexture = Hge->Texture_Load("particles.png");

	WARN_IF(!HeroTexture, "Texture 'particles.png' not found!");
		
	Sprite = new hgeSprite(HeroTexture, 96, 64, 32, 32);
	Sprite->SetColor(0xFFFFA000);
	Sprite->SetHotSpot(16, 16);
}

Hero::~Hero(void)
{
	delete Sprite;
	Hge->Texture_Free(HeroTexture);
}

void Hero::Move(Vector2D step)
{
	Step = step;
}

void Hero::Update(float deltaTime)
{
	Vector2D newLocation = Location + Step * deltaTime;
	bool bFree = true;

	// for each actors in the world
	for (std::set<IActor*>::iterator it = OwnerWorld->AllActors.begin(); it != OwnerWorld->AllActors.end(); it++)
	{
		// if the actor is not this Hero // test: and it is a static actor
		if ((*it) != this && (*it)->GetType() == AT_Static)
		{
			// get an actor's AABB (axis-aligned bounding box)
			BoundingBox box = (*it)->GetBoundingBox();
			// if the actor's AABB intersects with the Hero's AABB (in new Hero location)
			if ((box.MinX < newLocation.X + Size.X/2 && newLocation.X - Size.X/2 < box.MaxX)
				&&
				(box.MinY < newLocation.Y + Size.Y/2 && newLocation.Y - Size.Y/2 < box.MaxY))
			{
				// actor's path is not free
				bFree = false;
			}
		}
	}

	// if actor's path is free
	if (bFree)
	{
		// accept new position of the hero
		Location = newLocation;
	}
	
	UpdateCollision();
	Step = ZeroVector;
}

void Hero::UpdateCollision()
{
	ColideBox = BoundingBox(Location - Size/2, Location + Size/2);
}

void Hero::Render(Vector2D shift, Rotator angle)
{
	if (Sprite != NULL)
	{
		Sprite->RenderEx(shift.X, shift.Y, (Direction + angle).GetValue());
	}
}

void Hero::StartShoting(Vector2D targetLocation)
{
	if (ArmedWeapon != NULL)
	{
		ArmedWeapon->StartShooting(Location ,targetLocation);
	}
}

void Hero::StopShoting()
{
	if (ArmedWeapon != NULL)
	{
		ArmedWeapon->StopShooting();
	}
}

void Hero::GiveWeapon(Weapon *weap)
{
	ArmedWeapon = weap;
	weap->SetOwnerWorld(OwnerWorld);
	ArmedWeapon->SetEquipped(true);
}