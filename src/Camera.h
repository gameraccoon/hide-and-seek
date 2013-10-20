#ifndef CAMERA_H
#define CAMERA_H

#include "../src/Globals.h"
#include "../src/Vector2D.h"
#include "../src/Actor.h"
#include "../src/World.h"

class Camera
{
public:
	Camera(HGE *hge, World* world, Vector2D location);
	~Camera(void);
	void Render();
	void SetLocation(Vector2D newLocation);
	void SetResolution(Vector2D newResolution);
	Vector2D GetResolution();
	void SetRotation(Rotator angle);
	void ShowCollizion(bool bShow);
protected:
	void RenderCollisionBoxes();
	World* BrowsableWorld;
	Vector2D Location;
	/** Screen resolution */
	Vector2D Resolution;
	/** World location of camera center */
	Vector2D CenterPos;
	/** Rotation */
	float Angle;
	/** Pointer of the HGE subsystem */
	HGE *Hge;
	/** Sprite of the actor */
	hgeSprite *Sprite;
	/** Texture of all Camera sprites */
	HTEXTURE CamTexture;
	/** is shows collizion */
	bool bShowCollizion;
};

#endif