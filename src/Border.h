#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "../src/Vector2D.h"

class Border
{
public:
	Border(Vector2D a, Vector2D b);
	~Border(void);
	Vector2D GetNormal();
	Vector2D GetA();
	Vector2D GetB();
	void SetA(Vector2D a);
	void SetB(Vector2D b);
protected:
	Vector2D A;
	Vector2D B;
	Vector2D Normal;
};

#endif