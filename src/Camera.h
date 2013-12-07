#ifndef CAMERA_H
#define CAMERA_H

#include <hge.h>
#include <hgeresource.h>

#include "../src/Globals.h"
#include "../src/Vector2D.h"
#include "../src/Actor.h"
#include "../src/World.h"

class Camera
{
public:
	Camera(World* world, Vector2D resolution, Vector2D location);
	virtual ~Camera(void);
	/** Get pointer to the texture which will be used to render */
	HTEXTURE GetRenderTexture();
	/** Render of all objects thats seen by camera */
	void Render();
	/** Set new location of camera in the world */
	virtual void SetLocation(Vector2D newLocation);
	/** Get screen resolution */
	Vector2D GetResolution();
	/** Set new angle of camera rotation */
	void SetRotation(Rotator angle);
	/** Switch showing bounding boxes */
	void ShowAABB(bool bShow);
	/** Switch showing fog */
	void ShowFog(bool bShow);
	/** Switch showing light ceners */
	void ShowLights(bool bShow);
	/** Switch showing shadows */
	void ShowShadows(bool bShow);
	/** Switch showing normals */
	void ShowHulls(bool bShow);
	/** Project point from world coordinates to screen coordinates */
	Vector2D Project(Vector2D worldPoint);
	Vector2D ProjectFrom(Vector2D worldPoint, Vector2D projectionCenter);
	/** Project point from screen coordinates to the world coordinates */
	Vector2D DeProject(Vector2D screenPoint);
protected:
	/** Render all seen actors */
	void RenderActors(Vector2D lightPos);
	/** Render collision AABB to screen */
	void RenderCollisionBoxes();
	/** Render fog for the camera (fog of war) */
	virtual void RenderFog();
	/** Render shadows of player view */
	void RenderShadows();
	void RenderLightShadows(Vector2D lightPos);
	/** Render hulls */
	void RenderHulls();
	/** Render lights centers */
	void RenderLights();
	/** World which render this camera*/
	World* BrowsableWorld;
	/** Camera location in the world */
	Vector2D Location;
	/** Screen resolution */
	Vector2D Resolution;
	/** Shift camera center of screen left-top angle in screen coordinates */
	Vector2D CenterPos;
	/** Rotation */
	float Angle;
	/** Pointer of the HGE subsystem */
	HGE *Hge;
	/** Sprite of the actor */
	hgeSprite *CollisionSprite;
	/** Texture of all Camera sprites */
	HTEXTURE CamTexture;
	/** Texture of fog */
	HTEXTURE FogTexture;
	/** Render target */
	HTARGET RenderTarget;
	/** Sprite of fog */
	hgeSprite *FogSprite;
	/** Is camera shows AABB? */
	bool bShowAABB;
	/** Maximum distans of renderable objects from camera */
	float ShownSize;
	/** Is camera render fog? */
	bool bRenderFog;
	/** Fog texture width */
	float FogWidth;
	/** Fog's size */
	float FogScale;
	/** Is camera render shadows? */
	bool bRenderShadows;
	/** Is camera shows borders of objects? */
	bool bShowBorders;
	/** Is camera shows centers of lights? */
	bool bShowLights;
	/** Helper variable */
	HTARGET Zone;
private:
	/** Helper method. Drawing quad on screen */
	void DrawQuad(Vector2D first, Vector2D second, Vector2D third, Vector2D fourth);
	/** Drawing penumbra triangle */
	void DrawPenumbra(Vector2D first, Vector2D second, Vector2D third);
};

#endif