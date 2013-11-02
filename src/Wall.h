#ifndef WALL_H
#define WALL_H

#include "../src/Actor.h"

class Wall : public Actor
{
public:
	Wall(World *ownerWorld, Vector2D location, Vector2D size);
	~Wall(void);
	/** Dummy (wall do nothing) */
	void Update(float deltaTime);
	/** Render the actor in the current location */
	void Render(Vector2D shift, Rotator angle);
private:
	/** */
	void UpdateCollision();
	/** Wall lenght and height */
	Vector2D Size;
	/** Texture for sprites */
	HTEXTURE WallTexture;
};

#endif