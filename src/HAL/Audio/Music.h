#pragma once

#include <string>
#include "HAL/Base/Resource.h"

struct _Mix_Music;
typedef struct _Mix_Music Mix_Music;

namespace Audio
{
	class Music : public HAL::Resource
	{
	public:
		Music() = default;
		explicit Music(const std::string& path);
		~Music() override;

		Mix_Music* getRawMusic() const;

		bool isValid() const override;

	private:
		Mix_Music* mMusic = nullptr;
	};
}
