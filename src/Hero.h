#ifndef HERO_H
#define HERO_H

#include "BaseProperties.h"

class Hero
{
private:
	/** Положение игрока в мировом пространстве" */
	Vector Location;
	/** Точка к которой движется герой */
	Vector NextLocation;
	/** Скорость перемещения героя в юнитах */
	float Speed;
public:
	Hero(void);
	virtual ~Hero(void);

	/** Произвести запланнированные для героя действия */
	void Process(int deltatime);

	/** Установить новое положение героя в мировом пространстве */
	void SetLocation(Vector location);
	/** Получить положение героя */
	Vector GetLocation();
	/** Заставить героя перемещаться к новой позиции */
	void MoveTo(Vector newLocation);
};

#endif