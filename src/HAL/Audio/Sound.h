#pragma once

#include "Base/String/Path.h"

#include "HAL/Base/Resource.h"

struct Mix_Chunk;

namespace Audio
{
	class Sound : public HAL::Resource
	{
	public:
		Sound() = default;
		explicit Sound(const ResourcePath& path);
		~Sound() override;

		Mix_Chunk* getRawSound() const;

		bool isValid() const override;

	private:
		Mix_Chunk* mSound = nullptr;
	};
}
