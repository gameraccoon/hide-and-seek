#include "DirectionArrow.h"

#include "GraphicLoader.h"

DirectionArrow::DirectionArrow(HGE *hge) : centerLocation(ZERO_VECTOR),
	direction(0.0f)
{
	this->hge = hge;

	this->arrowSprite = GraphicLoader::Instance().getSprite("arrow");
	this->arrowSprite->SetColor(0xFF00AA00);

	bDrawable = false;
}

DirectionArrow::~DirectionArrow(void)
{
}

void DirectionArrow::setScreenLocation(const Vector2D& scrLocation)
{
	this->centerLocation = scrLocation;
}

void DirectionArrow::setDirection(Rotator direction)
{
	this->bDrawable = true;
	this->direction = direction;
}


void DirectionArrow::setVDirection(Vector2D vectDirection)
{
	if (vectDirection != ZERO_VECTOR)
	{
		this->bDrawable = true;
		this->direction = vectDirection.rotation();
	}
	else
	{
		bDrawable = false;
	}
}

void DirectionArrow::setColor(DWORD color)
{
	this->arrowSprite->SetColor(color);
}

void DirectionArrow::render() const
{
	if (!this->bDrawable)
		return;
	
	Vector2D location = this->centerLocation + Vector2D(this->direction) * 50;
	this->arrowSprite->RenderEx(location.x, location.y, this->direction.getValue());
}

Vector2D DirectionArrow::getScreenLocation() const
{
	return this->centerLocation;
}

bool DirectionArrow::click()
{
	return false;
}
