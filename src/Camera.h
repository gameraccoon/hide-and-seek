#ifndef CAMERA_H
#define CAMERA_H

#include "../src/Vector2D.h"
#include "../src/Actor.h"
#include "../src/World.h"

class Camera
{
public:
	Camera(World* world, Vector2D location);
	~Camera(void);
	void Render();
	void SetLocation(Vector2D newLocation);
	void SetResolution(Vector2D newResolution);
	Vector2D GetResolution();
private:
	World* BrowsableWorld;
	Vector2D Location;
	/** Screen resolution */
	Vector2D Resolution;
	/** World cocation of camera center */
	Vector2D CenterPos;
};

#endif