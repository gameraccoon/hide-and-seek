#include "DirectionArrow.h"

DirectionArrow::DirectionArrow(void) : CenterLocation(ZeroVector)
{
	ArrowTexture = Hge->Texture_Load("arrow.png");

	WARN_IF(!ArrowTexture, "Texture 'arrow.png' not found!");
		
	ArrowSprite = new hgeSprite(ArrowTexture, 0, 0, 32, 32);
	ArrowSprite->SetColor(0xFF00AA00);
	ArrowSprite->SetHotSpot(16, 16);
}

DirectionArrow::~DirectionArrow(void)
{
	delete ArrowSprite;
	Hge->Texture_Free(ArrowTexture);
}

void DirectionArrow::SetCenter(Vector2D newCenter)
{
	CenterLocation = newCenter;
}

/*void DirectionArrow::SetDirection(Rotator newDirection)
{
	Direction = newDirection;
}*/

void DirectionArrow::Render()
{
	//ArrowSprite->RenderEx(CenterLocation.X, CenterLocation.Y, Direction);
}
