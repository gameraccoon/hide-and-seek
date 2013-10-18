#include "Wall.h"


Wall::Wall(HGE *hge, Vector2D location, Vector2D size) : Actor(hge, location)
{
	Lenght = size.X;
	Weight = size.Y;

	WallTexture = Hge->Texture_Load("testTexture.png");

	WARN_IF(!WallTexture, "Texture 'testTexture.png' not found!");

	Sprite = new hgeSprite(WallTexture, 0, 0, 126, 126);
	Sprite->SetColor(0xFFFFFFFF);
	Sprite->SetHotSpot(16, 16);
}


Wall::~Wall(void)
{
}

void Wall::Update(float deltaTime)
{
}

void Wall::Render(Vector2D shift, Rotator angle)
{
	if (Sprite != NULL)
	{
		Sprite->RenderEx(shift.X, shift.Y, Direction + angle, Lenght/126.0f, Weight/126.0f);
	}
}