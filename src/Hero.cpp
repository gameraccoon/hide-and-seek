#include "Hero.h"

Hero::Hero(World *ownerWorld, HGE *hge, Vector2D location) : Actor(ownerWorld, hge, location), Step(ZeroVector), Size(32.0f, 32.0f)
{
	Type = AT_Living;

	UpdateCollision();

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
	Vector2D newLocation = Location + Step * deltaTime;
	
	for (std::set<IActor*>::iterator it = OwnerWorld->AllActors.begin(); it != OwnerWorld->AllActors.end(); it++)
	{
		if ((*it) != this)
		{
			BoundingBox box = (*it)->GetBoundingBox();
			if ((box.MinX < newLocation.X + Size.X/2 && newLocation.X - Size.X/2 < box.MaxX)
				&&
				(box.MinY < newLocation.Y + Size.Y/2 && newLocation.Y - Size.Y/2 < box.MaxY))
			{
				// mirror?
			}
			else
			{
				Location = newLocation;
			}
		}
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
		Sprite->RenderEx(shift.X, shift.Y, Direction + angle);
	}
}
