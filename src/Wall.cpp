#include "Wall.h"

#include "../src/ActorFactory.h"

// unnamed namespase to hide from another places
namespace
{
	// specific factory
	IActor* CreateWall(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new Wall(world, location, scale);
	}

	const std::string WALL_ID = "Wall";

	// register specific factory in actor factory
	const bool registered = ActorFactory::RegisterActor(WALL_ID, CreateWall);
}

Wall::Wall(World *ownerWorld, Vector2D location, Vector2D scale) : Actor(ownerWorld, location, Rotator(0.f))
{
	Type = AT_Static;

	const int spritelSize = 126;

	OriginalSize = Vector2D(20, 20);
	SetScale(scale);

	Geometry.Points.insert(Geometry.Points.end(), -CalculatedSize/2);
	Geometry.Points.insert(Geometry.Points.end(), (CalculatedSize/2).MirrorV());
	Geometry.Points.insert(Geometry.Points.end(), CalculatedSize/2);
	Geometry.Points.insert(Geometry.Points.end(), (CalculatedSize/2).MirrorH());
	Geometry.Generate();

	UpdateCollision();

	WallTexture = Hge->Texture_Load("testTexture.png");

	WARN_IF(!WallTexture, "Texture 'testTexture.png' not found!");

	Sprite = new hgeSprite(WallTexture, 0, 0, (float)spritelSize, (float)spritelSize);
	Sprite->SetColor(0xFFFFFFFF);
	Sprite->SetHotSpot(spritelSize/2 + 1, spritelSize/2 + 1);

	ClassID = WALL_ID;
}

Wall::~Wall(void)
{
	delete Sprite;
	Hge->Texture_Free(WallTexture);
}

void Wall::Update(float deltatime)
{
	Actor::Update(deltatime);
}

void Wall::UpdateCollision()
{
	ColideBox = BoundingBox(Location - CalculatedSize/2, Location + CalculatedSize/2);
}

void Wall::Render(Vector2D shift, Rotator angle)
{
	if (Sprite != NULL)
	{
		Sprite->RenderEx(shift.X, shift.Y, (Direction + angle).GetValue(), CalculatedSize.X/126.0f, CalculatedSize.Y/126.0f);
	}
}

void Wall::TakeDamage(float damageValue, Vector2D impulse)
{
	// do nothing
}