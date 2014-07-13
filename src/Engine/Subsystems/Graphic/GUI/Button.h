#ifndef BUTTON_H
#define BUTTON_H

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgeresource.h>

#include <string>

#include "IGraphicElement.h"

class Button : public IGraphicElement
{
public:
	Button(HGE *hge, std::string spriteNormal, std::string spriteHover);
	virtual ~Button();
	virtual void render() const override;
	virtual Vector2D getScreenLocation() const override;
	virtual void setScreenLocation(const Vector2D& scrLocation) override;
	/**
	 * @return false if we missed
	 */
	virtual bool click() override;

	virtual bool checkHovered(const Vector2D& mousePos) override;
private:
	HGE *hge;
	
	Vector2D screenLocation;

	hgeSprite *normalSprite;
	hgeSprite *hoverSprite;
};

#endif