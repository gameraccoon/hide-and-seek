#pragma once
#include "Camera.h"
class FloatingCamera : public Camera
{
public:
	FloatingCamera(HGE *hge, World* world, Vector2D location);
	~FloatingCamera(void);
	void SetLocation(Vector2D newLocation);
	void SetResolution(Vector2D newResolution);
	void SetCenterShift(Vector2D shift);
protected:
	Vector2D Shift;
};

