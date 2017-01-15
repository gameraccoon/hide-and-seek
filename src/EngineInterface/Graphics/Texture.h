#pragma once

#include <memory>

#include "Resource.h"
#include <EngineInterface/EngineFwd.h>

class Texture : public Resource
{
public:
	using Ptr = std::unique_ptr<Texture>;

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
	Texture(IUseCounter* useCounter, const Base* base);
	Texture(const Texture&) = default;
	Texture(Texture&&) = default;
	virtual ~Texture();

	int Texture::GetHeight() const;
	int Texture::GetWidth() const;

	void Draw(int x, int y);

private:
	const Texture::Base* mBase;
};
