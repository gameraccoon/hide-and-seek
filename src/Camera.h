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
	void SetRotation(Rotator angle);
protected:
	World* BrowsableWorld;
	Vector2D Location;
	/** Screen resolution */
	Vector2D Resolution;
	/** World location of camera center */
	Vector2D CenterPos;
	/** Rotation */
	float Angle;
};

#endif