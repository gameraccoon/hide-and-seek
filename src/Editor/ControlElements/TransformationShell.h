#ifndef TRANSFORMATION_SHELL_H
#define TRANSFORMATION_SHELL_H

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgeresource.h>

#include <Engine/Core/Vector2D.h>
#include <Engine/Core/Rotator.h>
#include <Engine/Core/IActor.h>

#include <Engine/Subsystems/Graphic/GUI/Button.h>

class TransformationShell : public IGraphicElement
{
public:
	enum ModificationEvent
	{
		Move
		,Rotate
		,Scale
		,None
	};

	TransformationShell(HGE *hge, IActor *controlledActor);
	virtual ~TransformationShell();
	/** Set center of arrow in world coordinates */
	virtual void setScreenLocation(const Vector2D& scrLocation) override;
	virtual Vector2D getScreenLocation() const override;
	/** Render arrow to screen */
	virtual void render() const override;
	virtual bool click() override;
	virtual bool checkHovered(const Vector2D& mousePos) override;
	ModificationEvent checkButton(const Vector2D& mousePos);

private:
	/** Pointer of the HGE subsystem */
	HGE *hge;
	Vector2D screenLocation;

	IActor *controlledActor;
	
	Button moveBtn;
	Button scaleBtn;
	Button rotateBtn;
};

#endif
