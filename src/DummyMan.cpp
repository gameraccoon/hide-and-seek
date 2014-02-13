#include "DummyMan.h"

DummyMan::DummyMan(World *ownerWorld, Vector2D location) : Actor(ownerWorld, location, Rotator(0.f)),
														size(32.0f, 32.0f)
{
	this->type = AT_Living;

	this->speed = 12.0f;

	this->geometry.points.insert(this->geometry.points.end(), -this->size / 2);
	this->geometry.points.insert(this->geometry.points.end(), (this->size / 2).mirrorV());
	this->geometry.points.insert(this->geometry.points.end(), this->size / 2);
	this->geometry.points.insert(this->geometry.points.end(), (this->size / 2).mirrorH());
	this->geometry.generate();
	
	this->updateCollision();

	this->manTexture = this->hge->Texture_Load("hero.png");

	WARN_IF(!this->manTexture, "Texture 'hero.png' not found!");
		
	this->sprite = new hgeSprite(this->manTexture, 0, 0, 32, 32);
	this->sprite->SetColor(0xFFFFFFFF);
	this->sprite->SetHotSpot(16, 16);
}

DummyMan::~DummyMan(void)
{
	delete this->sprite;
	this->hge->Texture_Free(this->manTexture);
}

void DummyMan::move(Vector2D step)
{
	this->location += step;
	this->updateCollision();
}

void DummyMan::updateCollision()
{
	this->colideBox = BoundingBox(this->location - this->size/2, this->location + this->size/2);
}

void DummyMan::render(Vector2D shift, Rotator angle)
{
	if (this->sprite != NULL)
	{
		this->sprite->RenderEx(shift.x, shift.y, (this->direction + angle).getValue());
	}
}

void DummyMan::startShoting(Vector2D targetLocation)
{
	if (this->armedWeapon != NULL)
	{
		this->armedWeapon->startShooting(this->location ,targetLocation);
	}
}

void DummyMan::stopShoting()
{
	if (this->armedWeapon != NULL)
	{
		this->armedWeapon->stopShooting();
	}
}

void DummyMan::giveWeapon(Weapon *weap)
{
	this->armedWeapon = weap;
	weap->setOwnerWorld(this->ownerWorld);
	this->armedWeapon->setEquipped(true);
}

void DummyMan::takeDamage(float damageValue,Vector2D impulse)
{
	this->move(impulse);
}
