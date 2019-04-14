#include "Modules/RayTrace.h"

#include <Components/CollisionComponent.h>
#include <Components/TransformComponent.h>

#include <algorithm>

namespace RayTrace
{
	namespace
	{
		const int LEFT_BIT = 1;
		const int RIGHT_BIT = 2;
		const int BOTTOM_BIT = 4;
		const int TOP_BIT = 8;
	
		//
		const float EPS = 1E-4f;
	}
	inline int getDotCode(const BoundingBox &box, const Vector2D &dot)
	{
		return (((dot.x < box.minX) ? LEFT_BIT : 0)
				| ((dot.x > box.maxX) ? RIGHT_BIT : 0)
				| ((dot.y < box.minY) ? BOTTOM_BIT : 0)
				| ((dot.y > box.maxY) ? TOP_BIT : 0));
	}

	inline float SignedArea(const Vector2D &a, const Vector2D &b, const Vector2D &c)
	{
		return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	}

	inline void Swap(float &a, float &b)
	{
		float temp;
		temp = a;
		a = b;
		b = temp;
	}

	inline float Det(const float a, const float b, const float c, const float d)
	{
		return a * d - b * c;
	}

	inline bool IsBetween(const float a, const float b, const float c)
	{
		return std::min(a, b) <= c + EPS && c <= std::max(a, b) + EPS;
	}

	inline bool IsAAIntersect(const float a, const float b, const float c, const float d)
	{
		float a1 = a;
		float b1 = b;
		float c1 = c;
		float d1 = d;

		if (a1 > b1)
			Swap(a1, b1);
		if (c1 > d1)
			Swap(c1, d1);

		return std::max(a1, c1) <= std::min(b1, d1);
	}

	inline bool checkIntersect2Lines(const Vector2D &A1, const Vector2D &A2, const Vector2D &B1, const Vector2D &B2)
	{
		if (IsAAIntersect(A1.x, A2.x, B1.x, B2.x)
			&& IsAAIntersect(A1.y, A2.y, B1.y, B2.y)
			&& SignedArea(A1, A2, B1) * SignedArea(A1, A2, B2) <= 0
			&& SignedArea(B1, B2, A1) * SignedArea(B1, B2, A2) <= 0)
		{
			return true;
		}
		return false;
	}

	inline bool checkIntersectAABBLine(const BoundingBox &box, const Vector2D &first, const Vector2D &last)
	{
		float l = box.minX;
		float t = box.minY;
		float r = box.maxX;
		float b = box.maxY;

		float x1 = first.x;
		float y1 = first.y;
		float x2 = last.x;
		float y2 = last.y;

		// ToDo: optimize it for axis-aligned borders
		if (checkIntersect2Lines(Vector2D(l, t), Vector2D(l, b), Vector2D(x1, y1), Vector2D(x2, y2))
			||
			checkIntersect2Lines(Vector2D(r, t), Vector2D(r, b), Vector2D(x1, y1), Vector2D(x2, y2))
			||
			checkIntersect2Lines(Vector2D(l, t), Vector2D(r, t), Vector2D(x1, y1), Vector2D(x2, y2))
			||
			checkIntersect2Lines(Vector2D(l, b), Vector2D(r, b), Vector2D(x1, y1), Vector2D(x2, y2)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	Vector2D GetPointIntersect2Lines(const Vector2D &A1, const Vector2D &A2, const Vector2D &B1, const Vector2D &B2)
	{
		float DA1 = A1.y - A2.y;
		float DB1 = A2.x - A1.x;
		float DC1 = -DA1 * A1.x - DB1 * A1.y;
		float DA2 = B1.y - B2.y;
		float DB2 = B2.x - B1.x;
		float DC2 = -DA2 * B1.x - DB2 * B1.y;

		float zn = RayTrace::Det(DA1, DB1, DA2, DB2);
	
		// if lines are not parallel
		if (zn < -EPS || zn > EPS)
		{
			float x = -RayTrace::Det(DC1, DB1, DC2, DB2) / zn;
			float y = -RayTrace::Det(DA1, DC1, DA2, DC2) / zn;

			return Vector2D(x, y);
		}

		// if lines not intersected
		return ZERO_VECTOR;
	}

	bool FastTrace(World* /*world*/, const Vector2D& /*startPoint*/, const Vector2D& /*endPoint*/)
	{
//		for (const auto& currentActor : world->getAllActors())
//		{
//			if (currentActor->getType() == ActorType::Light || currentActor->getType() == ActorType::Special)
//			{
//				continue;
//			}

//			auto actorCollisionComponent = currentActor->getSingleComponent<CollisionComponent>();
//			if (actorCollisionComponent == nullptr)
//			{
//				continue;
//			}

//			auto actorTransformComponent = currentActor->getSingleComponent<TransformComponent>();
//			if (actorTransformComponent == nullptr)
//			{
//				continue;
//			}
		
//			// get bounding box of current actor
//			BoundingBox box = actorCollisionComponent->getBoundingBox();
//			// get cohen's code for start point
//			int a = RayTrace::getDotCode(box, startPoint);
//			// get cohen's code for end point
//			int b = RayTrace::getDotCode(box, endPoint);

//			// if points on some side of BB
//			if ((a & b) != 0)
//			{
//				// go to the next actor
//				continue;
//			}

//			// if raytrace intersect AABB of this actor
//			if (((a == 0 || b == 0)	&& (a | b) != 0)	// one point is in BB another is out BB
//				||
//				((a | b) == 3 || (a | b) == 12)			// points on opposite sides of BB // 0011 or 1100
//				||
//				RayTrace::checkIntersectAABBLine(box, startPoint, endPoint))	// other cases of intersection
//			{
//				//return true;
//				// get hull of current actor
//				const Hull *hull = actorCollisionComponent->getGeometry();

//				// for each border
//				for (auto &border : hull->borders)
//				{
//					Vector2D actorsLocation(actorTransformComponent->getLocation());
//					// if ray have different direction with normal
//					if (abs((border.getNormal().rotation() - (endPoint - startPoint).rotation()).getValue()) <= PI/2)
//						continue;

//					// if raytrace intersect this border
//					if (RayTrace::checkIntersect2Lines(actorsLocation + border.getA(), actorsLocation + border.getB(),
//						startPoint, endPoint))
//					{
//						return true;
//					}
//				}
//			}
//		}
		return false;
	}

//	IActor* trace(World * world, const Vector2D &startPoint, const Vector2D &endPoint,
//		Vector2D *outPoint, Vector2D *outNormal)
//	{
//		// nearest actor that intersects ray
//		IActor *nearestActor = nullptr;
//		// hitpoint of nearest actor
//		Vector2D nearestHitPoint(ZERO_VECTOR);
//		Vector2D nearestNormal(ZERO_VECTOR);
//		float minRayLength = (startPoint - endPoint).size() + 20.0f;

//		// for each actor in the world
//		for (const auto& currentActor : world->getAllActors())
//		{
//			if (currentActor->getType() == ActorType::Light || currentActor->getType() == ActorType::Special)
//			{
//				continue;
//			}

//			auto actorCollisionComponent = currentActor->getSingleComponent<CollisionComponent>();
//			if (actorCollisionComponent == nullptr)
//			{
//				continue;
//			}

//			auto actorTransformComponent = currentActor->getSingleComponent<TransformComponent>();
//			if (actorTransformComponent == nullptr)
//			{
//				continue;
//			}

//			// get bounding box of current actor
//			BoundingBox box = actorCollisionComponent->getBoundingBox();
//			// get cohen's code for start point
//			int a = RayTrace::getDotCode(box, startPoint);
//			// get cohen's code for end point
//			int b = RayTrace::getDotCode(box, endPoint);

//			// if points on some side of BB
//			if ((a & b) != 0)
//			{
//				// go to the next actor
//				continue;
//			}

//			// if raytrace intersect AABB of this actor
//			if (((a == 0 || b == 0)	&& (a | b) != 0)	// one point is in BB another is out BB
//				||
//				((a | b) == 3 || (a | b) == 12)			// points on opposite sides of BB // 0011 or 1100
//				||
//				RayTrace::checkIntersectAABBLine(box, startPoint, endPoint))	// other cases of intersection
//			{
//				//return true;
//				// get hull of current actor
//				const Hull *hull = actorCollisionComponent->getGeometry();

//				// for each border
//				for (auto &border : hull->borders)
//				{
//					Vector2D actorsLocation(actorTransformComponent->getLocation());

//					// if ray have different direction with normal
//					if (abs((border.getNormal().rotation() - (endPoint - startPoint).rotation()).getValue()) <= PI/2)
//						continue;

//					// if raytrace intersect this border
//					if (!RayTrace::checkIntersect2Lines(actorsLocation + border.getA(), actorsLocation + border.getB(), startPoint, endPoint))
//						continue;

//					Vector2D hitLocation = RayTrace::getPointIntersect2Lines(actorsLocation + border.getA(), actorsLocation + border.getB(),
//						startPoint, endPoint);
							
//					float rayLength = (startPoint - hitLocation).size();

//					// if currentActor nearer than last
//					if (rayLength < minRayLength)
//					{
//						minRayLength = rayLength;
//						nearestActor = currentActor.get();
//						nearestHitPoint = hitLocation;
//						nearestNormal = border.getNormal();
//					}
//				}
//			}
//		}

//		// return all values
//		if (outPoint != nullptr)
//		{
//			*outPoint = nearestHitPoint;
//		}

//		if (outNormal != nullptr)
//		{
//			*outNormal = nearestNormal;
//		}

//		return nearestActor;
//	}
}
