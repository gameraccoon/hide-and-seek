#ifndef HULL_H
#define HULL_H

#include <vector>

#include "../src/Vector2D.h"
#include "../src/Border.h"

class Hull
{
public:
	Hull(void);
	~Hull(void);
	/** Corners of a hull's borders */
	std::vector<Vector2D> Points;
	/** Borders of hull (calculates from Points) */
	std::vector<Border> Borders;
	/** Calc borders from points */
	void Generate();
};

#endif