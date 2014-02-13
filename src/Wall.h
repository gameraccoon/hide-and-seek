#ifndef WALL_H
#define WALL_H

#include "../src/Actor.h"

class Wall : public Actor
{
public:
	Wall(World *ownerWorld, Vector2D location, Vector2D size);
	~Wall(void);
	/** Dummy (wall do nothing) */
	void Update(float deltatime);
	/** Render the actor in the current location */
	void Render(Vector2D shift, Rotator angle);
	/** Try to take some damage to the wall =) */
	virtual void TakeDamage(float damageValue, Vector2D impulse);
private:
	/** */
	void UpdateCollision();
	/** Texture for sprites */
	HTEXTURE WallTexture;
};

#endif