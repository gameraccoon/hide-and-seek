#include "RayTrace.h"

#include "Hero.h"

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
		| ((dot->X > box->MaxX) ? RightBit : 0)
		| ((dot->Y < box->MinY) ? BottomBit : 0)
		| ((dot->Y > box->MaxY) ? TopBit : 0));
}

inline float SignedArea(Vector2D a, Vector2D b, Vector2D c)
{
	return (b.X - a.X) * (c.Y - a.Y) - (b.Y - a.Y) * (c.X - a.X);
}

inline void Swap(float &a, float &b)
{
	float temp;
	temp = a;
	a = b;
	b = temp;
}

//
const double EPS = 1E-4;
 
inline float Det(float a, float b, float c, float d)
{
	return a * d - b * c;
}
 
inline bool IsBetween(float a, float b, double c)
{
	return min(a, b) <= c + EPS && c <= max(a, b) + EPS;
}

inline bool IsAAIntersect(float a, float b, float c, float d)
{
	if (a > b)
		Swap(a, b);
	if (c > d)
		Swap(c, d);

	return max(a, c) <= min(b, d);
}

bool RayTrace::CheckIntersect2Lines(Vector2D A1, Vector2D A2, Vector2D B1, Vector2D B2)
{
	if (IsAAIntersect(A1.X, A2.X, B1.X, B2.X)
		&& IsAAIntersect(A1.Y, A2.Y, B1.Y, B2.Y)
		&& SignedArea(A1, A2, B1) * SignedArea(A1, A2, B2) <= 0
		&& SignedArea(B1, B2, A1) * SignedArea(B1, B2, A2) <= 0)
	{
				return true;
	}
	return false;
}

Vector2D RayTrace::GetPointIntersect2Lines(Vector2D A1, Vector2D A2, Vector2D B1, Vector2D B2)
{
	float DA1 = A1.Y - A2.Y;
	float DB1 = A2.X - A1.X;
	float DC1 = -DA1 * A1.X - DB1 * A1.Y;
	float DA2 = B1.Y - B2.Y;
	float DB2 = B2.X - B1.X;
	float DC2 = -DA2 * B1.X - DB2 * B1.Y;

	float zn = Det(DA1, DB1, DA2, DB2);
	
	// if lines are not parallel
	if (zn != 0)
	{
		float x = (float) (-Det(DC1, DB1, DC2, DB2) * 1. / zn);
		float y = (float) (-Det(DA1, DC1, DA2, DC2) * 1. / zn);

		if (IsBetween(A1.X, A2.X, x) && IsBetween(A1.Y, A2.Y, y)
			&& IsBetween(B1.X, B2.X, x) && IsBetween(B1.Y, B2.Y, y))
		{
			return Vector2D(x, y);
		}
	}

	// if lines not intersected
	return ZeroVector;
}

bool RayTrace::CheckIntersectAABBLine(const BoundingBox* box, const Vector2D* first, const Vector2D* last)
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
	if (CheckIntersect2Lines(Vector2D(l, t), Vector2D(l, b), Vector2D(x1, y1), Vector2D(x2, y2))
		||
		CheckIntersect2Lines(Vector2D(r, t), Vector2D(r, b), Vector2D(x1, y1), Vector2D(x2, y2))
		||
		CheckIntersect2Lines(Vector2D(l, t), Vector2D(r, t), Vector2D(x1, y1), Vector2D(x2, y2))
		||
		CheckIntersect2Lines(Vector2D(l, b), Vector2D(r, b), Vector2D(x1, y1), Vector2D(x2, y2)))
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

bool RayTrace::FastTrace()
{
	IActor *itActor;
	// for each actor in the world
	for (std::set<IActor*>::iterator it = OwnerWorld->AllActors.begin(); it != OwnerWorld->AllActors.end(); it++)
	{
		itActor = *it;
		if (itActor->GetType() == AT_Static)
		{
			// get bounding box of current actor
			BoundingBox box = itActor->GetBoundingBox();
			// get cohen's code for start point
			byte a = GetDotCode(&box, &StartPoint);
			// get cohen's code for end point
			byte b = GetDotCode(&box, &EndPoint);

			// one point is in BB another is out BB
			if ((a == 0 || b == 0) && ((a & b) == 0))
			{
				//return true;
			}

			// if points on some side of BB
			if ((a & b) != 0)
			{
				// go to the next actor
				continue;
			}

			// if points on opposite sides of BB
			if ((a | b) == 3 || (a | b) == 12) // 0011 or 1100 
			{
				//return true;
			}

			// if raytrace intersect AABB of this actor
			if (CheckIntersectAABBLine(&box, &StartPoint, &EndPoint))
			{
				return true;
				// get hull of current actor
                Hull *hull = itActor->GetHull();

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
