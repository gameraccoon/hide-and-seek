#include "Bullet.h"


Bullet::Bullet(World *ownerWorld, HGE *hge, Vector2D location) : Actor(ownerWorld, hge, location)
{
	Speed = 10.0f;	
	Type = AT_Bullet;

	Speed = 12.0f;

	BulletTexture = Hge->Texture_Load("bullet.png");

	WARN_IF(!BulletTexture, "Texture 'bullet.png' not found!");
		
	Sprite = new hgeSprite(BulletTexture, 96, 64, 32, 32);
	Sprite->SetColor(0xFFFFA000);
	Sprite->SetHotSpot(16, 16);
}

Bullet::~Bullet(void)
{
	delete Sprite;
	Hge->Texture_Free(BulletTexture);
}

void Bullet::Update(float deltaTime)
{
	Location += deltaTime * Speed * Vector2D(Direction);
}

void Bullet::Render(Vector2D shift, Rotator angle)
{
	if (Sprite != NULL)
	{
		Sprite->RenderEx(shift.X, shift.Y, (Direction + angle).GetValue());
	}
}
