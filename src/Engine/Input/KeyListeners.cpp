#include "KeyListeners.h"

ButtonSwitcher::ButtonSwitcher(HGE *hge, byte key, bool active)
{
	bActive = active;
	this->key = key;
	this->hge = hge;
}

ButtonSwitcher::~ButtonSwitcher()
{
}

bool ButtonSwitcher::isActive()
{
	return this->bActive;
}

byte ButtonSwitcher::getKey()
{
	return this->key;
}

void ButtonSwitcher::check()
{
	if (this->hge->Input_GetKeyState(this->key))
	{
		if (!this->bPressed)
		{
			this->bPressed = true;
			this->switchState();
			this->pressed();
		}
	}
	else
	{
		if (this->bPressed)
		{
			this->bPressed = false;
			this->released();
		}
	}
}

void ButtonSwitcher::switchState()
{
	this->bActive = !this->bActive;
}


void ButtonListeners::addListener(ButtonSwitcher *listener)
{
	this->listeners.insert(this->listeners.begin(), listener);
}

void ButtonListeners::check()
{
	for (int i = 0; i < (int) this->listeners.size(); i++)
	{
		this->listeners[i]->check();
	}
}

bool ButtonListeners::isActive(int key)
{
	for (int i = 0; i < (int) this->listeners.size(); i++)
	{
		if (this->listeners[i]->getKey() == key)
		{
			return this->listeners[i]->isActive();
		}
	}

	return false;
}

void ButtonListeners::destruct()
{
	for (std::vector<ButtonSwitcher*>::iterator it = this->listeners.begin(); it != this->listeners.end(); it++)
	{
		delete (*it);
		(*it) = nullptr;
	}
}
