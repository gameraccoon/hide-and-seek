#include "DirectionArrow.h"

DirectionArrow::DirectionArrow(HGE *hge) : CenterLocation(ZeroVector), Direction(0.0f)
{
	Hge = hge;

	ArrowTexture = Hge->Texture_Load("arrow.png");

	WARN_IF(!ArrowTexture, "Texture 'arrow.png' not found!");
		
	ArrowSprite = new hgeSprite(ArrowTexture, 0, 0, 32, 32);
	ArrowSprite->SetColor(0xFF00AA00);
	ArrowSprite->SetHotSpot(16, 16);

	bDrawable = false;
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

void DirectionArrow::SetDirection(Rotator newDirection)
{
	bDrawable = true;
	Direction = newDirection;
}


void DirectionArrow::SetVDirection(Vector2D vectDirection)
{
	if (vectDirection != ZeroVector)
	{
		bDrawable = true;
		Direction = vectDirection.GetRotation();
	}
	else
	{
		bDrawable = false;
	}
}


void DirectionArrow::Render()
{
	if (bDrawable)
	{
		Vector2D location = CenterLocation + Vector2D(Direction) * 50;
		ArrowSprite->RenderEx(location.X, location.Y, Direction.GetValue());
	}
}
