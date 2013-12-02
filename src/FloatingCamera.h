#ifndef FLOATINGCAMERA_H
#define FLOATINGCAMERA_H

#include "Camera.h"

class FloatingCamera : public Camera
{
public:
	FloatingCamera(World* world, Vector2D resolution, Vector2D location);
	~FloatingCamera(void);
	/** Set new location of this camera in the world */
	void SetLocation(Vector2D newLocation);
	/** Set shift of center of the screen */
	void SetCenterShift(Vector2D shift);
protected:
	void RenderFog();
	Vector2D Shift;
};

#endif