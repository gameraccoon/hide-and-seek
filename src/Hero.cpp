#include "Hero.h"

Hero::Hero(Vector2D location) : Actor(location), Step(ZeroVector)
{
	Speed = 12.0f;

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
	Location += Step * deltaTime;
	Step = ZeroVector;
}

void Hero::Render()
{
	if (Sprite != NULL)
	{
		Sprite->RenderEx(Location.X, Location.Y, Direction);
	}
}