#pragma once

#include <string>

#include "HAL/Base/Resource.h"

struct Mix_Chunk;

namespace Audio
{
	class Sound : public HAL::Resource
	{
	public:
		Sound() = default;
		explicit Sound(const std::string& path);
		~Sound() override;

		Mix_Chunk* getRawSound() const;

		virtual bool isValid() const override;

	private:
		Mix_Chunk* mSound = nullptr;
	};
}
