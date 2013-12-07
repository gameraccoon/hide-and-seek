#include "Man.h"

Man::Man(World *ownerWorld, Vector2D location) : Actor(ownerWorld, location), Step(ZeroVector), Size(32.0f, 32.0f)
{
	Type = AT_Living;

	Speed = 12.0f;

	Geometry.Points.insert(Geometry.Points.end(), -Size / 2);
	Geometry.Points.insert(Geometry.Points.end(), (Size / 2).MirrorV());
	Geometry.Points.insert(Geometry.Points.end(), Size / 2);
	Geometry.Points.insert(Geometry.Points.end(), (Size / 2).MirrorH());
	Geometry.Generate();
	
	UpdateCollision();

	ManTexture = Hge->Texture_Load("hero.png");

	WARN_IF(!ManTexture, "Texture 'hero.png' not found!");
		
	Sprite = new hgeSprite(ManTexture, 0, 0, 32, 32);
	Sprite->SetColor(0xFFFFFFFF);
	Sprite->SetHotSpot(16, 16);
}

Man::~Man(void)
{
	delete Sprite;
	Hge->Texture_Free(ManTexture);
}

void Man::Move(Vector2D step)
{
	Step = step;
}

void Man::Rotate(Rotator newDirection)
{
	Direction = newDirection;
}

void Man::Update(float deltaTime)
{
	Vector2D newLocation = Location + Step * deltaTime;
	bool bFree = true;

	// for each actors in the world
	for (std::set<IActor*>::iterator it = OwnerWorld->AllActors.begin(); it != OwnerWorld->AllActors.end(); it++)
	{
		// if the actor is not this man // test: and it is a static actor
		if ((*it) != this && ((*it)->GetType() != AT_Light && (*it)->GetType() != AT_Special && (*it)->GetType() != AT_Bullet))
		{
			// get an actor's AABB (axis-aligned bounding box)
			BoundingBox box = (*it)->GetBoundingBox();
			// if the actor's AABB intersects with the Man's AABB (in new Man location)
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
		// accept new position of the man
		Location = newLocation;
	}
	
	UpdateCollision();
	Step = ZeroVector;
}

void Man::UpdateCollision()
{
	ColideBox = BoundingBox(Location - Size/2, Location + Size/2);
}

void Man::Render(Vector2D shift, Rotator angle)
{
	if (Sprite != NULL)
	{
		Sprite->RenderEx(shift.X, shift.Y, (Direction + angle).GetValue());
	}
}

void Man::StartShoting(Vector2D targetLocation)
{
	if (ArmedWeapon != NULL)
	{
		ArmedWeapon->StartShooting(Location ,targetLocation);
	}
}

void Man::StopShoting()
{
	if (ArmedWeapon != NULL)
	{
		ArmedWeapon->StopShooting();
	}
}

void Man::GiveWeapon(Weapon *weap)
{
	ArmedWeapon = weap;
	weap->SetOwnerWorld(OwnerWorld);
	ArmedWeapon->SetEquipped(true);
}

void Man::TakeDamage(float damageValue,Vector2D impulse)
{
	Move(impulse);
}