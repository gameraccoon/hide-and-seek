#include "KeyListeners.h"

ButtonSwitcher::ButtonSwitcher(HGE *hge, byte key, bool active)
{
	bActive = active;
	Key = key;
	Hge = hge;
}

bool ButtonSwitcher::IsActive()
{
	return bActive;
}

byte ButtonSwitcher::GetKey()
{
	return Key;
}

void ButtonSwitcher::Test()
{
	if (Hge->Input_GetKeyState(Key))
	{
		if (!bPressed)
		{
			bPressed = true;
			Switch();
			Pressed();
		}
	}
	else
	{
		if (bPressed)
		{
			bPressed = false;
			Released();
		}
	}
}

void ButtonSwitcher::Switch()
{
	bActive = !bActive;
}


void ButtonListeners::AddListener(ButtonSwitcher *listener)
{
	Listeners.insert(Listeners.begin(), listener);
}

void ButtonListeners::Test()
{
	for (int i = 0; i < Listeners.size(); i++)
	{
		Listeners[i]->Test();
	}
}

bool ButtonListeners::GetActive(int key)
{
	for (int i = 0; i < Listeners.size(); i++)
	{
		if (Listeners[i]->GetKey() == key)
		{
			return Listeners[i]->IsActive();
		}
	}

	return false;
}

void ButtonListeners::Destruct()
{
	for (std::vector<ButtonSwitcher*>::iterator it = Listeners.begin(); it != Listeners.end(); it++)
	{
		delete (*it);
		(*it) = NULL;
	}
}