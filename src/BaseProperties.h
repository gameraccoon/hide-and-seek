#ifndef BASE_PROPERTIES_H
#define BASE_PROPERTIES_H

// Стандартные компоненты, такие как значение NULL
//#include <cstddef>
// Включаем все функции, необходимые для работы с HGE
#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgeparticle.h>
// В дальнейшем нам понадобится функция sprintf()
//#include <stdio.h>

#include "States.h"
#include "Processes.h"
#include "Vector2D.h"
//#include "Hero.h"

/** Допустимая погрешность в положении объектов на экране */
const float PIXEL_ERROR = 0.9f;

#include <iostream>

using namespace std;
#define DEBUG

#if defined(DEBUG)
	#define WARN(message)				\
	{									\
		cout << "Warning: ";			\
		cout << message << endl;		\
	}

	#define WARN_IF(condition, message)	\
	{									\
		if (condition)					\
		{								\
			WARN(message);				\
		}								\
	}
#else
	#define WARN(message)
	#define WARN_IF(condition, message)
#endif

#endif
