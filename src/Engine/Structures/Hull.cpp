#include "Hull.h"


Hull::Hull(void)
{
}


Hull::~Hull(void)
{
}

void Hull::generate()
{
	this->borders.clear();
	int count = this->points.size();
	for (int i = 0; i < count; i++)
	{
		this->borders.insert(this->borders.end(), Border(this->points[i], this->points[(i + 1) % count]));
	}
}