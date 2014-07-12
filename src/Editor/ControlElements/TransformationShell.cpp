#include "TransformationShell.h"

#include <Debug/Log.h>

#include <HgeInterface/Graphics/GraphicLoader.h>

TransformationShell::TransformationShell(HGE *hge) : screenLocation(ZERO_VECTOR)
{
	this->hge = hge;

	this->sprite = GraphicLoader::Instance().getSprite("transform");
	this->sprite->SetColor(0xFFFFFFFF);
}

TransformationShell::~TransformationShell(void)
{
}

void TransformationShell::setScreenLocation(const Vector2D& scrLocation)
{
	this->screenLocation = scrLocation;
}

Vector2D TransformationShell::getScreenLocation() const
{
	return this->screenLocation;
}

void TransformationShell::render() const
{
	this->sprite->Render(this->screenLocation.x, this->screenLocation.y);
}

bool TransformationShell::click()
{
	return false;
}