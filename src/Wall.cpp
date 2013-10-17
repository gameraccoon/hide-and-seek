#include "Wall.h"


Wall::Wall(Vector2D location, Vector2D size) : Actor(location)
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

void Wall::Render()
{
	if (Sprite != NULL)
	{
		Sprite->RenderEx(Location.X, Location.Y, Direction, Lenght/126.0, Weight/126.0);
	}
}