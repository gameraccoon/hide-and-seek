#pragma once

#include <memory>

#include "Base/Resource.h"
#include <EngineFwd.h>

class Font : public Resource
{
public:
	class Base : public Resource::Base
	{
	public:
		Base(IUseCounter::Uid uid, Destructor destructor)
			: Resource::Base(uid, destructor)
		{
		}

		virtual ~Base();

		// because std::unique_ptr doesn't like fwd
		Engine::Internal::SdlSurface* surface;
		Engine::Internal::Engine* engine;
	};

public:
	Font(IUseCounter* useCounter, const Base* base);
	Font(const Font&) = default;
	Font(Font&&) = default;
	virtual ~Font();

	void Draw(const char* text, float x, float y, float rotation = 0.0f);

	virtual bool isValid() const override;

private:
	const Font::Base* mBase;
};
