#ifndef FLOATINGCAMERA_H
#define FLOATINGCAMERA_H

#include "Camera.h"

class FloatingCamera : public Camera
{
public:
	FloatingCamera(HGE* hge, World* world, Vector2D resolution, Vector2D location);
	~FloatingCamera(void);
	/** Set new location of this camera in the world */
	void setLocation(Vector2D newLocation);
	/** Set shift of center of the screen */
	void setCenterShift(Vector2D shift);
protected:
	void renderFog();
	Vector2D shift;
};

#endif
