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
	std::vector<Vector2D> Points;
	std::vector<Border> Borders;
	void Generate();
};

#endif