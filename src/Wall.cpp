#include "Wall.h"


Wall::Wall(World *ownerWorld, Vector2D location, Vector2D size) : Actor(ownerWorld, location), Size(size)
{
	Type = AT_Static;

	Geometry.Points.insert(Geometry.Points.end(), -Size/2);
	Geometry.Points.insert(Geometry.Points.end(), (Size/2).MirrorV());
	Geometry.Points.insert(Geometry.Points.end(), Size/2);
	Geometry.Points.insert(Geometry.Points.end(), (Size/2).MirrorH());
	Geometry.Generate();

	UpdateCollision();

	WallTexture = Hge->Texture_Load("testTexture.png");

	WARN_IF(!WallTexture, "Texture 'testTexture.png' not found!");

	Sprite = new hgeSprite(WallTexture, 0, 0, 126, 126);
	Sprite->SetColor(0xFFFFFFFF);
	Sprite->SetHotSpot(63, 63);
}

Wall::~Wall(void)
{
	delete Sprite;
	Hge->Texture_Free(WallTexture);
}

void Wall::Update(float deltaTime)
{
}

void Wall::UpdateCollision()
{
	ColideBox = BoundingBox(Location - Size/2, Location + Size/2);
}

void Wall::Render(Vector2D shift, Rotator angle)
{
	if (Sprite != NULL)
	{
		Sprite->RenderEx(shift.X, shift.Y, (Direction + angle).GetValue(), Size.X/126.0f, Size.Y/126.0f);
	}
}