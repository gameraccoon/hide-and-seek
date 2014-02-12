#include "DummyMan.h"

DummyMan::DummyMan(World *ownerWorld, Vector2D location) : Actor(ownerWorld, location, Rotator(0.f)),
														Size(32.0f, 32.0f)
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

DummyMan::~DummyMan(void)
{
	delete Sprite;
	Hge->Texture_Free(ManTexture);
}

void DummyMan::Move(Vector2D step)
{
	Location += step;
	UpdateCollision();
}

void DummyMan::UpdateCollision()
{
	ColideBox = BoundingBox(Location - Size/2, Location + Size/2);
}

void DummyMan::Render(Vector2D shift, Rotator angle)
{
	if (Sprite != NULL)
	{
		Sprite->RenderEx(shift.X, shift.Y, (Direction + angle).GetValue());
	}
}

void DummyMan::StartShoting(Vector2D targetLocation)
{
	if (ArmedWeapon != NULL)
	{
		ArmedWeapon->StartShooting(Location ,targetLocation);
	}
}

void DummyMan::StopShoting()
{
	if (ArmedWeapon != NULL)
	{
		ArmedWeapon->StopShooting();
	}
}

void DummyMan::GiveWeapon(Weapon *weap)
{
	ArmedWeapon = weap;
	weap->SetOwnerWorld(OwnerWorld);
	ArmedWeapon->SetEquipped(true);
}

void DummyMan::TakeDamage(float damageValue,Vector2D impulse)
{
	Move(impulse);
}