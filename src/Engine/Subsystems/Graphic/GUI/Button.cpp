#include "Button.h"

#include <HgeInterface/Graphics/GraphicLoader.h>

Button::Button(HGE *hge, std::string spriteNormal, std::string spriteHover) : screenLocation(ZERO_VECTOR)
{
	this->hge = hge;
	this->normalSprite = GraphicLoader::Instance().getSprite(spriteNormal);
}

Button::~Button()
{
}

void Button::render() const
{
	this->normalSprite->Render(this->screenLocation.x, this->screenLocation.y);
}

Vector2D Button::getScreenLocation() const
{
	return this->screenLocation;
}

void Button::setScreenLocation(const Vector2D& scrLocation)
{
	this->screenLocation = scrLocation;
}

bool Button::click()
{
	return false;
}

bool Button::checkHovered(const Vector2D& mousePos)
{
	return mousePos.x >= this->screenLocation.x - 16
		&& mousePos.y >= this->screenLocation.y - 16
		&& mousePos.x <= this->screenLocation.x + 16
		&& mousePos.y <= this->screenLocation.y + 16;
}