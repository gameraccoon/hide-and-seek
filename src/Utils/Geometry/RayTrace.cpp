#include "Base/precomp.h"

#include "Utils/Geometry/RayTrace.h"

#include <algorithm>

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"

#include "GameData/Core/Vector2D.h"
#include "GameData/Core/Rotator.h"
#include "GameData/Core/BoundingBox.h"
#include "GameData/World.h"

#include "Utils/Geometry/Collide.h"

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
	static inline int getDotCode(const BoundingBox& box, const Vector2D& dot)
	{
		return (((dot.x < box.minX) ? LEFT_BIT : 0)
				| ((dot.x > box.maxX) ? RIGHT_BIT : 0)
				| ((dot.y < box.minY) ? BOTTOM_BIT : 0)
				| ((dot.y > box.maxY) ? TOP_BIT : 0));
	}

	inline void Swap(float& a, float& b)
	{
		float temp;
		temp = a;
		a = b;
		b = temp;
	}

	static inline float Det(const float a, const float b, const float c, const float d)
	{
		return a * d - b * c;
	}

	static inline bool IsBetween(const float a, const float b, const float c)
	{
		return std::min(a, b) <= c + EPS && c <= std::max(a, b) + EPS;
	}

	static inline bool IsAAIntersect(const float a, const float b, const float c, const float d)
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

	bool AreLinesIntersect(const Vector2D& A1, const Vector2D& A2, const Vector2D& B1, const Vector2D& B2)
	{
		if (IsAAIntersect(A1.x, A2.x, B1.x, B2.x)
			&& IsAAIntersect(A1.y, A2.y, B1.y, B2.y)
			&& Collide::SignedArea(A1, A2, B1) * Collide::SignedArea(A1, A2, B2) <= 0
			&& Collide::SignedArea(B1, B2, A1) * Collide::SignedArea(B1, B2, A2) <= 0)
		{
			return true;
		}
		return false;
	}

	bool IsLineIntersectAABB(const BoundingBox& box, const Vector2D& first, const Vector2D& last)
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
		if (AreLinesIntersect(Vector2D(l, t), Vector2D(l, b), Vector2D(x1, y1), Vector2D(x2, y2))
			||
			AreLinesIntersect(Vector2D(r, t), Vector2D(r, b), Vector2D(x1, y1), Vector2D(x2, y2))
			||
			AreLinesIntersect(Vector2D(l, t), Vector2D(r, t), Vector2D(x1, y1), Vector2D(x2, y2))
			||
			AreLinesIntersect(Vector2D(l, b), Vector2D(r, b), Vector2D(x1, y1), Vector2D(x2, y2)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool FastTrace(World& world, const Vector2D& startPoint, const Vector2D& endPoint)
	{
		TupleVector<CollisionComponent*, TransformComponent*> components;
		// ToDo: choose only potentially intersected cells
		world.getSpatialData().getAllCellManagers().getComponents(components);

		for (auto [collision, transform] : components)
		{
			Vector2D transformedStartPoint = startPoint - transform->getLocation();
			Vector2D transformedEndPoint = endPoint - transform->getLocation();

			BoundingBox box = collision->getBoundingBox();
			// get Cohen's code for start point
			int codeA = RayTrace::getDotCode(box, transformedStartPoint);
			// get Cohen's code for end point
			int codeB = RayTrace::getDotCode(box, transformedEndPoint);

			// if the points on the same side of BB
			if ((codeA & codeB) != 0)
			{
				// skip to the next entity
				continue;
			}

			// if the raytrace intersects with AABB of this actor
			if (((codeA == 0 || codeB == 0)	&& (codeA | codeB) != 0)	// one point is in BB another is out BB
				||
				((codeA | codeB) == 3 || (codeA | codeB) == 12)			// points on opposite sides of BB // 0011 or 1100
				||
				RayTrace::IsLineIntersectAABB(box, transformedStartPoint, transformedEndPoint))	// other cases of intersection
			{
				const Hull& hull = collision->getGeometry();

				if (hull.type == HullType::Angular)
				{
					// for each border
					for (auto& border : hull.borders)
					{
						// if ray have opposite direction with normal
						if (abs((border.getNormal().rotation() - (transformedEndPoint - transformedStartPoint).rotation()).getValue()) <= PI/2)
						{
							continue;
						}

						// if the raytrace intersects with this border
						if (RayTrace::AreLinesIntersect(border.getA(), border.getB(), transformedStartPoint, transformedEndPoint))
						{
							return true;
						}
					}
				}
				else
				{
					Vector2D d = endPoint - startPoint;
					Vector2D f = startPoint - transform->getLocation();
					float r = hull.getRadius();

					float a = DotProduct(d, d);
					float b = 2.0f * DotProduct(f, d);
					float c = DotProduct(f, f) - r * r;

					float discriminant = b * b - 4 * a * c;
					if (discriminant >= 0)
					{
						discriminant = sqrt(discriminant);

						float t1 = (-b - discriminant) / (2.0f * a);

						if (t1 >= 0 && t1 <= 1)
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	TraceResult Trace(World& world, Vector2D startPoint, Vector2D endPoint)
	{
		TraceResult result;

		float minRayQLength = (startPoint - endPoint).qSize() + 20.0f;

		// ToDo: choose only potentially intersected cells
		world.getSpatialData().getAllCellManagers().forEachSpatialComponentSetWithEntity<CollisionComponent, TransformComponent>([&result, &minRayQLength, startPoint, endPoint](Entity entity, WorldCell* cell, CollisionComponent* collision, TransformComponent* transform)
		{
			Vector2D transformedStartPoint = startPoint - transform->getLocation();
			Vector2D transformedEndPoint = endPoint - transform->getLocation();

			BoundingBox box = collision->getBoundingBox();
			// get Cohen's code for start point
			int codeA = RayTrace::getDotCode(box, transformedStartPoint);
			// get Cohen's code for end point
			int codeB = RayTrace::getDotCode(box, transformedEndPoint);

			// if the points on the same side of BB
			if ((codeA & codeB) != 0)
			{
				// skip to the next entity
				return;
			}

			// if the raytrace intersects with AABB of this entity
			if (((codeA == 0 || codeB == 0)	&& (codeA | codeB) != 0)	// one point is in BB another is out BB
				||
				((codeA | codeB) == 3 || (codeA | codeB) == 12)			// points on opposite sides of BB // 0011 or 1100
				||
				RayTrace::IsLineIntersectAABB(box, transformedStartPoint, transformedEndPoint))	// other cases of intersection
			{
				const Hull& hull = collision->getGeometry();

				if (hull.type == HullType::Angular)
				{
					// for each border
					for (auto& border : hull.borders)
					{
						// if ray have opposite direction with normal
						if (abs((border.getNormal().rotation() - (transformedEndPoint - transformedStartPoint).rotation()).getValue()) <= PI/2)
						{
							continue;
						}

						// if the raytrace intersects with this border
						if (!RayTrace::AreLinesIntersect(border.getA(), border.getB(), transformedStartPoint, transformedEndPoint))
						{
							continue;
						}

						Vector2D hitLocation = RayTrace::GetPointIntersect2Lines(border.getA(), border.getB(),
							transformedStartPoint, transformedEndPoint);

						float rayQLength = (transformedStartPoint - hitLocation).qSize();

						// if currentActor closer than the previous one
						if (rayQLength < minRayQLength)
						{
							minRayQLength = rayQLength;
							result.hasHit = true;
							result.hitEntity.entity = entity;
							result.hitEntity.cell = cell->getPos();
							result.hitPoint = transform->getLocation() + hitLocation;
							result.hitBorderNormal = border.getNormal();
						}
					}
				}
				else
				{
					Vector2D d = endPoint - startPoint;
					Vector2D f = startPoint - transform->getLocation();
					float r = hull.getRadius();

					float a = DotProduct(d, d);
					float b = 2.0f * DotProduct(f, d);
					float c = DotProduct(f, f) - r * r;

					float discriminant = b * b - 4 * a * c;
					if (discriminant >= 0)
					{
						discriminant = sqrt(discriminant);

						float t1 = (-b - discriminant) / (2.0f * a);

						if (t1 >= 0 && t1 <= 1)
						{
							float rayLength = d.size() * t1;
							float rayQLength = rayLength * rayLength;
							if (rayQLength < minRayQLength)
							{
								minRayQLength = rayQLength;
								result.hasHit = true;
								result.hitEntity.entity = entity;
								result.hitEntity.cell = cell->getPos();
								result.hitPoint = startPoint + d * t1;
								result.hitBorderNormal = (result.hitPoint - transform->getLocation()).unit();
							}
						}
					}
				}
			}
		});

		return result;
	}

	Vector2D GetPointIntersect2Lines(const Vector2D& A1, const Vector2D& A2, const Vector2D& B1, const Vector2D& B2)
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
}
