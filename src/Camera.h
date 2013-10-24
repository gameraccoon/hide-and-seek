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
	void ShowShadows(bool bShow);
	/** Project point from world to screen */
	Vector2D Project(Vector2D);
protected:
	void RenderActors();
	void RenderCollisionBoxes();
	virtual void RenderFog();
	void RenderShadows();
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
	float FogWidth;
	float FogScale;
	/** */
	bool bRenderShadows;
private: void DrawQuad(Vector2D first, Vector2D second, Vector2D third, Vector2D fourth);
};

#endif