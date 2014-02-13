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
	const bool registered = ActorFactory::registerActor(WALL_ID, CreateWall);
}

Wall::Wall(World *ownerWorld, Vector2D location, Vector2D scale) : Actor(ownerWorld, location, Rotator(0.f))
{
	this->type = AT_Static;

	const int spritelSize = 126;

	this->originalSize = Vector2D(20, 20);
	this->setScale(scale);

	this->geometry.points.insert(this->geometry.points.end(), -this->calculatedSize/2);
	this->geometry.points.insert(this->geometry.points.end(), (this->calculatedSize/2).mirrorV());
	this->geometry.points.insert(this->geometry.points.end(), this->calculatedSize/2);
	this->geometry.points.insert(this->geometry.points.end(), (this->calculatedSize/2).mirrorH());
	this->geometry.generate();

	this->updateCollision();

	this->wallTexture = this->hge->Texture_Load("testTexture.png");

	WARN_IF(!this->wallTexture, "Texture 'testTexture.png' not found!");

	this->sprite = new hgeSprite(this->wallTexture, 0, 0, (float)spritelSize, (float)spritelSize);
	this->sprite->SetColor(0xFFFFFFFF);
	this->sprite->SetHotSpot(spritelSize/2 + 1, spritelSize/2 + 1);

	this->classID = WALL_ID;
}

Wall::~Wall(void)
{
	delete this->sprite;
	this->hge->Texture_Free(this->wallTexture);
}

void Wall::update(float deltatime)
{
	Actor::update(deltatime);
}

void Wall::updateCollision()
{
	this->colideBox = BoundingBox(this->location - this->calculatedSize/2, this->location + this->calculatedSize/2);
}

void Wall::render(Vector2D shift, Rotator angle)
{
	if (this->sprite != NULL)
	{
		this->sprite->RenderEx(shift.x, shift.y,
			(this->direction + angle).getValue(), this->calculatedSize.x/126.0f, this->calculatedSize.y/126.0f);
	}
}

void Wall::takeDamage(float damageValue, Vector2D impulse)
{
	// do nothing
}
