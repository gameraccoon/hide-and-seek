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
	/** Render of all objects thats seen by camera */
	void Render();
	/** Set new location of camera in the world */
	virtual void SetLocation(Vector2D newLocation);
	/** Set screen resolution */
	virtual void SetResolution(Vector2D newResolution);
	/** Get screen resolution */
	Vector2D GetResolution();
	/** Set new angle of camera rotation */
	void SetRotation(Rotator angle);
	/** Switch showing bounding boxes */
	void ShowAABB(bool bShow);
	/** Switch showing fog */
	void ShowFog(bool bShow);
	/** Switch showing shadows */
	void ShowShadows(bool bShow);
	/** Switch showing normals */
	void ShowHulls(bool bShow);
	/** Project point from world coordinates to screen coordinates */
	Vector2D Project(Vector2D);
protected:
	/** Render all seen actors */
	void RenderActors();
	/** Render collision AABB to screen */
	void RenderCollisionBoxes();
	/** Render fog (fog of war) */
	virtual void RenderFog();
	/** Render shadows of player view */
	void RenderShadows();
	/** Render hulls */
	void RenderHulls();
	/** World which render this camera*/
	World* BrowsableWorld;
	/** Camera location in the world */
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
	/** Texture of fog */
	HTEXTURE FogTexture;
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
private:
	/** Helper method. Drawing quad on screen */
	void DrawQuad(Vector2D first, Vector2D second, Vector2D third, Vector2D fourth);
};

#endif