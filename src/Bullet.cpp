#include "Bullet.h"


Bullet::Bullet(World *ownerWorld, Vector2D location, Vector2D targetLocation) :
	Actor(ownerWorld, location, (targetLocation - location).rotation()),
	direction((targetLocation - location).rotation())
{
	this->speed = 10.0f;	
	this->type = AT_Bullet;

	this->speed = 450.0f;

	this->ownerWorld = ownerWorld;

	this->bulletTexture = this->hge->Texture_Load("bullet.png");

	WARN_IF(!this->bulletTexture, "Texture 'bullet.png' not found!");
	
	this->sprite = new hgeSprite(this->bulletTexture, 0, 0, 32, 32);
	this->sprite->SetColor(0xFFFFFFFF);
	this->sprite->SetHotSpot(16, 16);
}

Bullet::~Bullet(void)
{
	delete this->sprite;
	this->hge->Texture_Free(this->bulletTexture);
}

void Bullet::update(float deltatime)
{
	Vector2D newLocation = this->location + deltatime * this->speed * Vector2D(this->direction);
	
	WARN_IF(!this->ownerWorld, "Not assigned OwnerWorld for bullet");

	RayTrace ray(this->ownerWorld, this->location, newLocation);
	Vector2D traceLocation(ZERO_VECTOR);
	IActor *trasedActor = ray.trace(&traceLocation);

	if (trasedActor == NULL)
	{
		this->location = newLocation;
	}
	else
	{
		trasedActor->takeDamage(10, Vector2D(this->direction) * this->speed);
		this->speed = 0.0f;
		this->destroy();
	}

	// bullet will be destroyed after 10 second
	if (this->lifetime > 10.f)
	{
		this->destroy();
	}
}

void Bullet::render(Vector2D shift, Rotator angle)
{
	if (this->sprite != NULL)
	{
		this->sprite->RenderEx(shift.x, shift.y, (this->direction + angle).getValue(), 0.3f, 0.3f);
	}
}

void Bullet::updateCollision()
{
}

void Bullet::takeDamage(float damageValue,Vector2D impulse)
{
	// do nothing for now
}
