#include "Input/KeyListeners.h"

namespace SystemInterface
{
	namespace Input
	{
        ButtonSwitcher::ButtonSwitcher(Engine *, char key, bool active)
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

        char ButtonSwitcher::getKey()
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
            for (size_t i = 0, count = mListeners.size(); i < count; i++)
			{
				mListeners[i]->check();
			}
		}

		bool ButtonListeners::isActive(int key)
		{
            for (size_t i = 0, count = mListeners.size(); i < count; i++)
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
	}
}
