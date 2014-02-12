#include "Bullet.h"


Bullet::Bullet(World *ownerWorld, Vector2D location, Vector2D targetLocation) :
																Actor(ownerWorld, location, (targetLocation - location).GetRotation()),
																Direction((targetLocation - location).GetRotation())
{
	Speed = 10.0f;	
	Type = AT_Bullet;

	Speed = 450.0f;

	Destroyed = false;

	OwnerWorld = ownerWorld;

	BulletTexture = Hge->Texture_Load("bullet.png");

	WARN_IF(!BulletTexture, "Texture 'bullet.png' not found!");
	
	Sprite = new hgeSprite(BulletTexture, 0, 0, 32, 32);
	Sprite->SetColor(0xFFFFFFFF);
	Sprite->SetHotSpot(16, 16);
}

Bullet::~Bullet(void)
{
	delete Sprite;
	Hge->Texture_Free(BulletTexture);
}

void Bullet::Update(float deltaTime)
{
	Vector2D newLocation = Location + deltaTime * Speed * Vector2D(Direction);
	
	WARN_IF(!OwnerWorld, "Not assigned OwnerWorld for bullet");

	RayTrace ray(OwnerWorld, Location, newLocation);
	Vector2D traceLocation(ZeroVector);
	IActor *trasedActor = ray.Trace(&traceLocation);

	if (trasedActor == NULL)
	{
		Location = newLocation;
	}
	else
	{
		trasedActor->TakeDamage(10, Vector2D(Direction) * Speed);
		Location = Vector2D(350.f, 250.f);// ToDo: destruct bullet
		Speed = 0.0f;
	}
}

void Bullet::Render(Vector2D shift, Rotator angle)
{
	if (Sprite != NULL)
	{
		Sprite->RenderEx(shift.X, shift.Y, (Direction + angle).GetValue(), 0.3f, 0.3f);
	}
}

void Bullet::UpdateCollision()
{
}


void Bullet::TakeDamage(float damageValue,Vector2D impulse)
{
	// do nothing for now
}