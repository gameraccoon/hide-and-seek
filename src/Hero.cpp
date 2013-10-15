#include "Hero.h"

Hero::Hero(Vector2D location) : Location(location), Step(ZeroVector)
{
	Speed = 1200.0f;

	HeroTexture = Hge->Texture_Load("particles.png");

	WARN_IF(!HeroTexture, "Texture 'particles.png' not found!");
		
	HeroSprite = new hgeSprite(HeroTexture, 96, 64, 32, 32);
	HeroSprite->SetColor(0xFFFFA000);
	HeroSprite->SetHotSpot(16, 16);
}

Hero::~Hero(void)
{
	delete HeroSprite;
	Hge->Texture_Free(HeroTexture);
}

void Hero::SetLocation(Vector2D newLocation)
{
	Location = newLocation;
}

Vector2D Hero::GetLocation()
{
	return Location;
}

void Hero::Move(Vector2D step)
{
	Step = step;
}

void Hero::Update(float deltaTime)
{
	Location += Step * deltaTime;
	Step = ZeroVector;
}

void Hero::Render()
{
	HeroSprite->Render(Location.X, Location.Y);
}