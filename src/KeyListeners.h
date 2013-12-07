#ifndef KEYLISTENERS_H
#define KEYLISTENERS_H

#include "../src/Globals.h"
#include <vector>

class ButtonSwitcher
{
public:
	ButtonSwitcher(HGE *hge, byte key, bool active);
	virtual ~ButtonSwitcher();
	bool IsActive();

	byte GetKey();

	void Check();

	virtual void Switch();

	virtual void Pressed() { }
	virtual void Released() { }

protected:
	bool bPressed;
	bool bActive;
	byte Key;
	HGE *Hge;
};

class ButtonListeners
{
public:
	void AddListener(ButtonSwitcher *listener);

	void Check();

	bool GetActive(int key);

	void Destruct();
private:
	std::vector<ButtonSwitcher*> Listeners;
};

#endif