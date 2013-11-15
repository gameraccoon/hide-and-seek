#include "RayTrace.h"

const byte RayTrace::LeftBit = 1;
const byte RayTrace::RightBit = 2;
const byte RayTrace::BottomBit = 4;
const byte RayTrace::TopBit = 8;

RayTrace::RayTrace(World * world, Vector2D start, Vector2D end) : StartPoint(start),
																EndPoint(end),
																Angle((end - start).GetRotation())
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

bool RayTrace::CheckIntersectAABBLine(const BoundingBox* box, const Vector2D* first, const Vector2D* last, Vector2D* outIntersectPoint)
{
	float l = box->MinX;
	float t = box->MinY;
	float r = box->MaxX;
	float b = box->MaxY;

	float x1 = first->X;
	float y1 = first->Y;
	float x2 = last->X;
	float y2 = last->Y;

	// ToDo: make it simpler (for axis-aligned borders)
	if (CheckIntersect2Lines(Vector2D(l, t), Vector2D(l, b), Vector2D(x1, y1), Vector2D(x2, y2), outIntersectPoint)
		||
		CheckIntersect2Lines(Vector2D(r, t), Vector2D(r, b), Vector2D(x1, y1), Vector2D(x2, y2), outIntersectPoint)
		||
		CheckIntersect2Lines(Vector2D(l, t), Vector2D(r, t), Vector2D(x1, y1), Vector2D(x2, y2), outIntersectPoint)
		||
		CheckIntersect2Lines(Vector2D(l, b), Vector2D(r, b), Vector2D(x1, y1), Vector2D(x2, y2), outIntersectPoint))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool RayTrace::CheckIntersectVertLineWithLine(Vector2D A1, Vector2D A2, float x, float minY, float maxY)
{
	return true;
}

bool RayTrace::CheckIntersectHoryLineWithLine(Vector2D A1, Vector2D A2, float y, float minX, float maxX)
{
	return true;
}

bool RayTrace::CheckIntersect2Lines(Vector2D A1, Vector2D A2, Vector2D B1, Vector2D B2, Vector2D *outIntersectionPoint)
{
	// if lines are parallel
	if ((A2 - A1).GetRotation() == (B2 - B1).GetRotation() || (A2 - A1).GetRotation() == (B1 - B2).GetRotation())
	{
		return false;
	}

	// calculate intersaction point from the system of two equations
	float intersectX = ((A2.X - A1.X)*(B2.X*B1.Y - B1.X*B2.Y) - (B2.X - B1.X)*(A2.X*A1.Y - A1.X*A2.Y))/((B2.X - B1.X)*(A2.Y - A1.Y) - (A2.X - A1.X)*(B2.Y - B1.Y));
	float intersectY = (A2.X*A1.Y - A1.X*A1.Y + (A2.Y - A1.Y)*intersectX)/(A2.X - A1.X);

	if (((A1.X <= intersectX && intersectX <= A2.X) && (B1.X <= intersectX && intersectX <= B2.X))
		||
		((A1.Y <= intersectY && intersectY <= A2.Y) && (B1.Y <= intersectY && intersectY <= B2.Y)))
	{
		if (outIntersectionPoint != NULL)
		{
			outIntersectionPoint->X = intersectX;
			outIntersectionPoint->Y = intersectY;
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool RayTrace::FastTrace()
{
	// for each actor in the world
	for (std::set<IActor*>::iterator it = OwnerWorld->AllActors.begin(); it != OwnerWorld->AllActors.end(); it++)
	{
		if ((*it)->GetType() == AT_Static)
		{
			// get bounding box of current actor
			BoundingBox box = (*it)->GetBoundingBox();
			// get cohen's code for start point
			byte a = GetDotCode(&box, &StartPoint);
			// get cohen's code for end point
			byte b = GetDotCode(&box, &EndPoint);
		
			// one point is in BB another is out BB
			if ((a == 0 || b == 0) && ((a & b) != 0))
			{
				return true;
			}

			// if points in some side
			if ((a | b) == 0)
			{
				// go to the next actor
				continue;
			}

			// if raytrace intersect AABB of this actor
			if (CheckIntersectAABBLine(&box, &StartPoint, &EndPoint))
			{
				return true;
				// get hull of current actor
				Hull *hull = (*it)->GetHull();
				float ax = StartPoint.X;
				float ay = StartPoint.Y;
				float bx = EndPoint.X;
				float by = EndPoint.Y;

				// for each border
				for (std::vector<Border>::iterator it2 = hull->Borders.begin(); it2 != hull->Borders.end(); it2++)
				{
					// if raytrace intersect some border of hull
					if (CheckIntersect2Lines(it2->GetA(), it2->GetB(), StartPoint, EndPoint))
					{
						return true;
					}
				}

			}
		}
	}
	return false;
}

//IActor* Trace(Vector2D *point, Vector2D *normal)
//{
//
//}