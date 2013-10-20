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
	void ShowCollision(bool bShow);
	void ShowFog(bool bShow);
protected:
	void RenderCollisionBoxes();
	virtual void RenderFog();
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
	hgeSprite *CollisionSprite;
	/** Texture of all Camera sprites */
	HTEXTURE CamTexture;
	HTEXTURE FogTexture;
	hgeSprite *FogSprite;
	/** is camera shows collision? */
	bool bShowCollision;
	/** maximum distans of renderable objects from camera */
	float ShownSize;
	/** is camera render fog? */
	bool bRenderFog;
	/** Fog texture width */
	int FogWidth;
	float FogScale;
};

#endif