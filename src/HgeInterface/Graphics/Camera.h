#ifndef CAMERA_H
#define CAMERA_H

#include <map>

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgeresource.h>

#include <Engine/Core/Vector2D.h>
#include <Engine/Core/World.h>
#include <Engine/Actors/Actor.h>

#include "GraphicLoader.h"

class Camera
{
public:
	Camera(HGE* hge, World* world, Vector2D resolution, Vector2D location);
	virtual ~Camera(void);
	/** Get pointer to the texture which will be used to render */
	HTEXTURE getRenderTexture();
	/** Render of all objects thats seen by camera */
	void render();
	/** Set new location of camera in the world */
	virtual void setLocation(Vector2D newLocation);
	/** Get screen resolution */
	Vector2D getResolution();
	/** Set new angle of camera rotation */
	void setRotation(Rotator angle);
	/** Switch showing bounding boxes */
	void showAABB(bool show);
	/** Switch showing fog */
	void showFog(bool show);
	/** Switch showing light ceners */
	void showLights(bool show);
	/** Switch showing shadows */
	void showShadows(bool show);
	/** Switch showing normals */
	void showHulls(bool show);
	/** Switch showing paths */
	void showPaths(bool show);
	/** Project point from world coordinates to screen coordinates */
	Vector2D project(Vector2D worldPoint);
	Vector2D projectFrom(Vector2D worldPoint, Vector2D projectionCenter);
	/** Project point from screen coordinates to the world coordinates */
	Vector2D deProject(Vector2D screenPoint);
protected:
	/** Render all seen actors */
	void renderActors(Vector2D lightPos);
	/** Render collision AABB to screen */
	void renderCollisionBoxes();
	/** Render fog for the camera (fog of war) */
	virtual void renderFog();
	/** Render shadows of player view */
	void renderShadows();
	void renderLightShadows(Vector2D lightPos);
	/** Render hulls */
	void renderHulls();
	/** Render lights centers */
	void renderLights();
	/** Render all path points and paths in the world */
	void renderPaths();
	/** World which render this camera*/
	World* browsableWorld;
	/** Camera location in the world */
	Vector2D location;
	/** Screen resolution */
	Vector2D resolution;
	/** Shift camera center of screen left-top angle in screen coordinates */
	Vector2D centerPos;
	/** Rotation */
	float angle;
	/** Pointer of the HGE subsystem */
	HGE *hge;
	/** Sprite of the actor */
	hgeSprite *collisionSprite;
	/** Texture of all Camera sprites */
	HTEXTURE camTexture;
	/** Texture of fog */
	HTEXTURE fogTexture;
	/** Render target */
	HTARGET renderTarget;
	/** Sprite of fog */
	hgeSprite *fogSprite;
	/** */
	GraphicLoader graphicLoader;
	/** Is camera shows AABB? */
	bool bShowAABB;
	/** Maximum distans of renderable objects from camera */
	float shownSize;
	/** Is camera render fog? */
	bool bRenderFog;
	/** Fog texture width */
	float fogWidth;
	/** Fog's size */
	float fogScale;
	/** Is camera render shadows? */
	bool bRenderShadows;
	/** Is camera shows borders of objects? */
	bool bShowBorders;
	/** Is camera shows paths */
	bool bShowPaths;
	/** Is camera shows centers of lights? */
	bool bShowLights;
	/** Helper variable */
	HTARGET zone;
private:
	/** Helper method. Drawing quad on screen */
	void drawQuad(Vector2D first, Vector2D second, Vector2D third, Vector2D fourth);
	/** Drawing penumbra triangle */
	void drawPenumbra(Vector2D first, Vector2D second, Vector2D third);
};

#endif
