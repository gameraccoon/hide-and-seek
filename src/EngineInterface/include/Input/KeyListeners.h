#pragma once

#include <EngineFwd.h>

#include <vector>

class ButtonSwitcher
{
public:
	ButtonSwitcher(Engine::Internal::Engine *engine, int8_t key, bool active);
	virtual ~ButtonSwitcher();
	bool isActive();

	int8_t getKey();

	void check();

	virtual void switchState();

	virtual void pressed() { }
	virtual void released() { }

protected:
	bool mIsPressed;
	bool mIsActive;
	int8_t mKey;
};

class ButtonListeners
{
public:
	void addListener(ButtonSwitcher *listener);

	void check();

	bool isActive(int key);

	void destruct();

private:
	std::vector<ButtonSwitcher*> mListeners;
};
