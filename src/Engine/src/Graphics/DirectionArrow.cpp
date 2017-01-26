#include "Graphics/DirectionArrow.h"

#include "Base/ResourceManager.h"
#include "Base/Engine.h"

DirectionArrow::DirectionArrow(Engine::Internal::Engine *)
	: mCenterLocation(ZERO_VECTOR)
	, mDirection(0.0f)
{
	/*engine = engine;

	arrowSprite = GraphicLoader::Instance().getSprite("arrow");
	arrowSprite->SetColor(0xFF00AA00);
	*/

	mIsDrawable = false;
}

DirectionArrow::~DirectionArrow()
{
}

void DirectionArrow::setScreenLocation(const Vector2D& scrLocation)
{
	mCenterLocation = scrLocation;
}

void DirectionArrow::setDirection(Rotator direction)
{
	mIsDrawable = true;
	mDirection = direction;
}


void DirectionArrow::setVDirection(Vector2D vectDirection)
{
	if (vectDirection != ZERO_VECTOR)
	{
		mIsDrawable = true;
		mDirection = vectDirection.rotation();
	}
	else
	{
		mIsDrawable = false;
	}
}

/*void DirectionArrow::setColor(DWORD color)
{
	mArrowSprite->SetColor(color);
}
*/
void DirectionArrow::render() const
{
	if (!mIsDrawable)
		return;
	
	Vector2D location = mCenterLocation + Vector2D(mDirection) * 50;
	//arrowSprite->RenderEx(location.x, location.y, direction.getValue());
}

Vector2D DirectionArrow::getScreenLocation() const
{
	return mCenterLocation;
}

bool DirectionArrow::click()
{
	return false;
}

bool DirectionArrow::checkHovered(const Vector2D&)
{
	return false;
}
