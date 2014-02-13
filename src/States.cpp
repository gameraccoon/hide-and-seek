#include "States.h"

#include <cstdlib>

void State::process()
{
	WARN("Function 'Process' not redefined in some state");
}

State::~State()
{
}

/**
 * Итератор состояния для стека состояний
 *
 * Нужен для скрытия деталей реализации стека состояний
 */
class StateIterator
{
public:
	/**
	 * Стандартный конструктор
	 *
	 * @param prewState итератор стейта который мы заменяем этим
	 * @param state стейт, для которого создаётся итератор
	 */
	StateIterator(StateIterator *prewState, State *state)
	{
		this->prewState = prewState;
		this->currentState = state;
	}

	~StateIterator()
	{
		WARN_IF(this->currentState == NULL, "Trying to destruct StateIterator which has no State");
		delete this->currentState;
	}

	/**
	 * Выполнить код состояния
	 */
	void process()
	{
		WARN_IF(!this->currentState, "Trying to process State which not exist in StateIterator");
		this->currentState->process();
	}

	/**
	 * @return Предыдущее состояние
	 */
	StateIterator* ReturnLast()
	{
		return this->prewState;
	}

private:
	/**
	 * Состояние, за которым закреплён итератор
	 */
	State *currentState;

	/**
	 * Предыдущее состояние
	 *
	 * Оно будет установлено, когда текущее состояние будет удалено.
	 */
	StateIterator *prewState;
};

StatesStack::StatesStack()
{
	this->head = NULL;
}

void StatesStack::push(State *newState)
{
	this->head = new StateIterator(this->head, newState);
}

void StatesStack::pop()
{
	// запоминаем ссылку на старое состояние
	StateIterator* oldHead = this->head;
	WARN_IF(!this->head, "Trying to 'Pop' from empty StatesStack. It is a fatal error.");
	// устонавливаем новое состояние
	this->head = this->head->ReturnLast();      
	delete oldHead;
}

void StatesStack::process()
{
	if (this->head != NULL)
	{
		this->head->process();
	}
}
