#include "TransformationShell.h"

#include <Debug/Log.h>

#include <HgeInterface/Graphics/GraphicLoader.h>

TransformationShell::TransformationShell(HGE *hge, IActor *controlledActor) : screenLocation(ZERO_VECTOR)
	, moveBtn(hge, "move", "moveHover")
	, rotateBtn(hge, "rotate", "rotateHover")
	, scaleBtn(hge, "scale", "scaleHover")
{
	this->hge = hge;

	this->controlledActor = controlledActor;
}

TransformationShell::~TransformationShell()
{
}

void TransformationShell::setScreenLocation(const Vector2D& scrLocation)
{
	this->screenLocation = scrLocation;
	
	this->moveBtn.setScreenLocation(scrLocation + Vector2D(-16, -16));
	this->scaleBtn.setScreenLocation(scrLocation + Vector2D(16, -16));
	this->rotateBtn.setScreenLocation(scrLocation + Vector2D(0, 16));
}

Vector2D TransformationShell::getScreenLocation() const
{
	return this->screenLocation;
}

void TransformationShell::render() const
{
	this->moveBtn.render();
	this->scaleBtn.render();
	this->rotateBtn.render();
}

bool TransformationShell::click()
{
	return false;
}

bool TransformationShell::checkHovered(const Vector2D& mousePos)
{
	return false;
}


TransformationShell::ModificationEvent TransformationShell::checkButton(const Vector2D& mousePos)
{
	if (this->moveBtn.checkHovered(mousePos))
	{
		return ModificationEvent::Move;
	}
	else if (this->scaleBtn.checkHovered(mousePos))
	{
		return ModificationEvent::Scale;
	}
	else if (this->rotateBtn.checkHovered(mousePos))
	{
		return ModificationEvent::Rotate;
	}
	else
	{
		return ModificationEvent::None;
	}
}