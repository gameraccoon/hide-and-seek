#include "Graphics/GUI/Button.h"

#include <Base/ResourceManager.h>

Button::Button(SystemInterface::Engine *, std::string spriteNormal, std::string spriteHover)
	: mScreenLocation(ZERO_VECTOR)
{
	//mNormalSprite = GraphicLoader::Instance().getSprite(spriteNormal);
}

void Button::render() const
{
	//mNormalSprite->Render(mScreenLocation.x, mScreenLocation.y);
}

Vector2D Button::getScreenLocation() const
{
	return mScreenLocation;
}

void Button::setScreenLocation(const Vector2D& scrLocation)
{
	mScreenLocation = scrLocation;
}

bool Button::click()
{
	return false;
}

bool Button::checkHovered(const Vector2D& mousePos)
{
	return mousePos.x >= mScreenLocation.x - 16
		&& mousePos.y >= mScreenLocation.y - 16
		&& mousePos.x <= mScreenLocation.x + 16
		&& mousePos.y <= mScreenLocation.y + 16;
}