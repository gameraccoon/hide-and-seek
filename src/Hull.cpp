#include "Hull.h"


Hull::Hull(void)
{
}


Hull::~Hull(void)
{
}

void Hull::Generate()
{
	Borders.clear();
	int count = Points.size();
	for (int i = 0; i < count; i++)
	{
		Borders.insert(Borders.end(), Border(Points[i], Points[(i + 1) % count]));
	}
}