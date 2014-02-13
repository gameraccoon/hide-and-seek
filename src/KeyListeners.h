#ifndef KEYLISTENERS_H
#define KEYLISTENERS_H

#include "../src/Globals.h"
#include <vector>

class ButtonSwitcher
{
public:
	ButtonSwitcher(HGE *hge, byte key, bool active);
	virtual ~ButtonSwitcher();
	bool isActive();

	byte getKey();

	void check();

	virtual void switchState();

	virtual void pressed() { }
	virtual void released() { }

protected:
	bool bPressed;
	bool bActive;
	byte key;
	HGE *hge;
};

class ButtonListeners
{
public:
	void addListener(ButtonSwitcher *listener);

	void check();

	bool isActive(int key);

	void destruct();
private:
	std::vector<ButtonSwitcher*> listeners;
};

#endif
