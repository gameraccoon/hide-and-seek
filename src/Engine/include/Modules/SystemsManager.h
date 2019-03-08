#pragma once

#include <vector>
#include <memory>
#include <Core/System.h>
#include <Core/World.h>

/**
 * Manager for game systems
 */
class SystemsManager
{
public:
	template <typename T, typename... Args>
	void registerSystem(Args&&... args)
	{
		mSystems.emplace_back(new T(std::forward<Args>(args)...));
	}

	void update(World* world, float dt);

private:
	std::vector<std::unique_ptr<System>> mSystems;
};
