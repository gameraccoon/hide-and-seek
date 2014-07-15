#ifndef HULL_H
#define HULL_H

#include <vector>

#include "../Core/Vector2D.h"
#include "Border.h"

class Hull
{
public:
	enum class Type
	{
		Angular
		,Circular
		,None
	};

	Hull(void);
	~Hull(void);
	/** Corners of a hull's borders */
	std::vector<Vector2D> points;
	/** Borders of hull (calculates from Points) */
	std::vector<Border> borders;
	/** Pre-calculated squared radius */
	float getQRadius() const;
	/** Get the radius */
	float getRadius() const;
	/** Set the radius */
	void setRadius(float newRadius);
	/** Calc borders from points */
	void generateBorders();

	Type type;
private:
	/** Squared radius */
	float qRadius;
	/** Radius for the circular hull */
	float radius;
};

#endif
