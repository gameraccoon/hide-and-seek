#include "RayTrace.h"

const byte RayTrace::LeftBit = 1;
const byte RayTrace::RightBit = 2;
const byte RayTrace::BottomBit = 4;
const byte RayTrace::TopBit = 8;

RayTrace::RayTrace(World * world, Vector2D start, Vector2D end) : StartPoint(start), EndPoint(end), Angle((end - start).GetRotation())
{
	OwnerWorld = world;
	RayLength = (end - start).Size();
}

RayTrace::RayTrace(World * world, Vector2D start, Rotator rotation, float length) : StartPoint(start), EndPoint(Vector2D(rotation) * length), Angle(rotation)
{
	OwnerWorld = world;
	RayLength = length;
}

RayTrace::~RayTrace(void)
{
}

byte RayTrace::GetDotCode(const BoundingBox *box, const Vector2D *dot)
{
	return (((dot->X < box->MinX) ? LeftBit : 0)
		+ ((dot->X > box->MaxX) ? RightBit : 0)
		+ ((dot->Y < box->MinY) ? BottomBit : 0)
		+ ((dot->Y > box->MaxY) ? TopBit : 0));
}

bool RayTrace::FastTrace()
{
	// for each figure
	for (std::set<IActor*>::iterator it = OwnerWorld->AllActors.begin(); it != OwnerWorld->AllActors.end(); it++)
	{
		// get bounding box of current figure
		BoundingBox box = (*it)->GetBoundingBox();
		// get cohen's code for start point
		byte a = GetDotCode(&box, &StartPoint);
		// get cohen's code for end point
		byte b = GetDotCode(&box, &EndPoint);
		
		// one point is in BB another is out BB
		if (a == 0 || b == 0)
		{
			return true;
		}

		// if points in some side
		if (a || b == 0)
		{
			return false;
		}

		// ToDo: place here some code =)
		
	}
	return true;
}

//IActor* Trace(Vector2D point, Vector2D normal)
//{
//
//}