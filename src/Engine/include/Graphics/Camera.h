#pragma once

#include <map>

#include <Core/Vector2D.h>
#include <Core/World.h>
#include <Core/Actor.h>

#include <EngineFwd.h>

#include <Components/LightComponent.h>

namespace Graphics
{
	class Texture;
}

namespace SystemInterface
{
	class ResourceManager;
}

class Camera
{
public:
	Camera(const World* world, Vector2D resolution, Vector2D location);
	virtual ~Camera();
	/** Get pointer to the texture which will be used to render */
	//HTEXTURE getRenderTexture() const;
	/** Render of all objects thats seen by camera */
	void render();
	/** Set new location of camera in the world */
	virtual void setLocation(const Vector2D &newLocation);
	/** Get screen resolution */
	Vector2D getResolution() const;
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
	Vector2D project(const Vector2D &worldPoint) const;
	Vector2D projectFrom(const Vector2D &worldPoint, const Vector2D &projectionCenter) const;
	/** Project point from screen coordinates to the world coordinates */
	Vector2D deProject(const Vector2D &screenPoint) const;

protected:
	/** Render all seen actors */
	void renderActors(const LightComponent::Ptr light);
	/** Render collision AABB to screen */
	void renderCollisionBoxes();
	/** Render dark ring like fog */
	virtual void renderFog(float width, float height, float size);
	/** Render shadows of player view */
	void renderShadows();
	void renderLightShadows(const LightComponent::Ptr light);
	/** Render hulls */
	void renderHulls();
	/** Render lights centers */
	void renderLights();
	/** Render all path points and paths in the world */
	void renderPaths();
	/** World which render this camera*/
	const World* mBrowsableWorld;
	/** Camera location in the world */
	Vector2D mLocation;
	/** Screen resolution */
	Vector2D mResolution;
	/** Shift camera center of screen left-top angle in screen coordinates */
	Vector2D mCenterPos;
	/** Rotation */
	float mAngle;
	/** Pointer of the Engine subsystem */
	std::unique_ptr<SystemInterface::Engine> mEngine;
	/** Sprite of the actor */
	//engineSprite* mCollisionSprite;
	/** Render target */
	//HTARGET mRenderTarget;
	/** Fog texture */
	Graphics::Texture* mFogTexture;
	/** Is camera shows AABB? */
	bool mIsShowAABB;
	/** Maximum distans of renderable objects from camera */
	float mShownSize;
	/** Is camera render fog? */
	bool mIsRenderFog;
	/** Fog texture width */
	float mFogWidth;
	/** Fog's size */
	float mFogScale;
	/** Is camera render shadows? */
	bool mIsRenderShadows;
	/** Is camera shows borders of objects? */
	bool mIsShowBorders;
	/** Is camera shows paths */
	bool mIsShowPaths;
	/** Is camera shows centers of lights? */
	bool mIsShowLights;
	/** Helper variable */
	//HTARGET zone;
private:
	/** Helper method. Drawing quad on screen */
	void drawQuad(const Vector2D &first, const Vector2D &second, const Vector2D &third, const Vector2D &fourth);
	/** Drawing penumbra triangle */
	void drawPenumbra(const Vector2D &first, const Vector2D &second, const Vector2D &third);
};
