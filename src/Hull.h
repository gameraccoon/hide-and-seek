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
	std::vector<Vector2D> points;
	/** Borders of hull (calculates from Points) */
	std::vector<Border> borders;
	/** Calc borders from points */
	void generate();
};

#endif
