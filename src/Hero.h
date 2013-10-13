#ifndef HERO_H
#define HERO_H

#include "..\src\BaseProperties.h"

class Hero
{
private:
	/** Положение игрока в мировом пространстве" */
	Vector2D Location;
	/** Точка к которой движется герой */
	Vector2D NextLocation;
	/** Скорость перемещения героя в юнитах */
	float Speed;
public:
	Hero(void);
	virtual ~Hero(void);

	/** Произвести запланнированные для героя действия */
	void Process(int deltatime);

	/** Установить новое положение героя в мировом пространстве */
	void SetLocation(Vector2D location);
	/** Получить положение героя */
	Vector2D GetLocation();
	/** Заставить героя перемещаться к новой позиции */
	void MoveTo(Vector2D newLocation);
};

#endif