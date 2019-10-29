#pragma once

#include "Base/String/Path.h"

#include "HAL/Base/Resource.h"

struct _Mix_Music;
typedef struct _Mix_Music Mix_Music;

namespace Audio
{
	class Music : public HAL::Resource
	{
	public:
		Music() = default;
		explicit Music(const ResourcePath& path);
		~Music() override;

		Mix_Music* getRawMusic() const;

		bool isValid() const override;

	private:
		Mix_Music* mMusic = nullptr;
	};
}
