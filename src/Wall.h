#ifndef WALL_H
#define WALL_H

#include "../src/Actor.h"

class Wall : public Actor
{
public:
	Wall(Vector2D location, Vector2D size);
	~Wall(void);
	void Update(float deltaTime);
	/** Render the actor in the current location */
	void Render(Vector2D shift);
private:
	/** Wall lenght */
	float Lenght;
	/** Wall weight */
	float Weight;
	/** Texture for sprites */
	HTEXTURE WallTexture;
};

#endif