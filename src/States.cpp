#include "States.h"

#include <cstdlib>

void State::Process()
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
		PrewState = prewState;
		CurrentState = state;
	}

	~StateIterator()
	{
		WARN_IF(CurrentState == NULL, "Trying to destruct StateIterator which has no State");
		delete CurrentState;
	}

	/**
	 * Выполнить код состояния
	 */
	void Process()
	{
		WARN_IF(!CurrentState, "Trying to process State which not exist in StateIterator");
		CurrentState->Process();
	}

	/**
	 * @return Предыдущее состояние
	 */
	StateIterator* ReturnLast()
	{
		return PrewState;
	}

private:
	/**
	 * Состояние, за которым закреплён итератор
	 */
	State *CurrentState;

	/**
	 * Предыдущее состояние
	 *
	 * Оно будет установлено, когда текущее состояние будет удалено.
	 */
	StateIterator *PrewState;
};

StatesStack::StatesStack()
{
	Head = NULL;
}

void StatesStack::Push(State *newState)
{
	Head = new StateIterator(Head, newState);
}

void StatesStack::Pop()
{
	// запоминаем ссылку на старое состояние
	StateIterator* oldHead = Head;
	WARN_IF(!Head, "Trying to 'Pop' from empty StatesStack. It is a fatal error.");
	// устонавливаем новое состояние
	Head = Head->ReturnLast();      
	delete oldHead;
}

void StatesStack::Process()
{
	if (Head != NULL)
	{
		Head->Process();
	}
}
