#pragma once

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

	Hull();
	~Hull();

	/** Pre-calculated squared radius */
	float getQRadius() const;
	/** Get the radius */
	float getRadius() const;
	/** Set the radius */
	void setRadius(float newRadius);
	/** Calc borders from points */
	void generateBorders();
	
	/** Corners of a hull's borders */
	std::vector<Vector2D> points;
	/** Borders of hull (calculates from Points) */
	std::vector<Border> borders;
	Type type;
private:
	/** Squared radius */
	float qRadius;
	/** Radius for the circular hull */
	float radius;
};
