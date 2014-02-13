#ifndef STATES_H
#define STATES_H

#include "../src/Globals.h"

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
	#define RELEASE
	#define WARN(message)
	#define WARN_IF(condition, message)
#endif

/**
 * Абстрактный интерфейс состояния
 */
class State
{
public:
	virtual ~State() = 0;
	/**
	 * Выполнить код состояния
	 *
	 * Функция для переопределения.
	 */
	virtual void process() = 0;
};

/**
 * Итератор состояния
 *
 * Нужен для скрытия деталей реализации стека состояний
 */
class StateIterator;

/**
 * Стек состояний
 */
class StatesStack
{
public:
	StatesStack();
	/**
	 * Установить новое состояние
	 *
	 * @param newState состояние, которое станет текущим
	 */
	void push(State *newState);
	
	/**
	 * Удалить текущее состояние
	 *
	 * Текущим станет предыдущее состояние
	 */
	void pop();

	/**
	 * Выполнить код состояния
	 */
	void process();

private:
	/** Верхушка стека */
	StateIterator *head;
};

#endif
