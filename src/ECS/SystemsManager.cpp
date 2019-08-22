#include "SystemsManager.h"

#include "Debug/Log.h"

void SystemsManager::update()
{
	for (std::unique_ptr<System>& system : mSystems)
	{
		system->update();
	}
}
