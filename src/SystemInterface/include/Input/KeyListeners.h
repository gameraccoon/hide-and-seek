#pragma once

#include <EngineFwd.h>

#include <vector>

namespace SystemInterface
{
	namespace Input
	{
		class ButtonSwitcher
		{
		public:
            ButtonSwitcher(Engine *engine, char key, bool active);
			virtual ~ButtonSwitcher();
			bool isActive();

            char getKey();

			void check();

			virtual void switchState();

			virtual void pressed() { }
			virtual void released() { }

		protected:
			bool mIsPressed;
			bool mIsActive;
            char mKey;
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
	}
}
