#include "Input/KeyListeners.h"

ButtonSwitcher::ButtonSwitcher(Engine::Internal::Engine *, int8_t key, bool active)
{
	mIsActive = active;
	mKey = key;
}

ButtonSwitcher::~ButtonSwitcher()
{
}

bool ButtonSwitcher::isActive()
{
	return mIsActive;
}

int8_t ButtonSwitcher::getKey()
{
	return mKey;
}

void ButtonSwitcher::check()
{
	/*if (engine->Input_GetKeyState(mKey))
	{
		if (!mIsPressed)
		{
			mIsPressed = true;
			switchState();
			pressed();
		}
	}
	else
	{
		if (mIsPressed)
		{
			mIsPressed = false;
			released();
		}
	}*/
}

void ButtonSwitcher::switchState()
{
	mIsActive = !mIsActive;
}


void ButtonListeners::addListener(ButtonSwitcher *listener)
{
	mListeners.insert(mListeners.begin(), listener);
}

void ButtonListeners::check()
{
	for (int i = 0; i < (int) mListeners.size(); i++)
	{
		mListeners[i]->check();
	}
}

bool ButtonListeners::isActive(int key)
{
	for (int i = 0; i < (int) mListeners.size(); i++)
	{
		if (mListeners[i]->getKey() == key)
		{
			return mListeners[i]->isActive();
		}
	}

	return false;
}

void ButtonListeners::destruct()
{
	for (std::vector<ButtonSwitcher*>::iterator it = mListeners.begin(); it != mListeners.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
}
