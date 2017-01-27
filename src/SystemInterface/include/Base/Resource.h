#pragma once

#include <functional>
#include <memory>

#include "IUseCounter.h"

namespace SystemInterface
{
	/**
	 * Base class for any resource type
	 */
	class Resource
	{
	public:
		class Base
		{
		public:
			using Destructor = std::function<void(Base*)>;

		public:
			Base(IUseCounter::Uid uid, Destructor destructor);
			virtual ~Base();

			IUseCounter::Uid uid;
			int useCount = 0;
			Destructor destructor;
		};

	public:
		Resource(IUseCounter* useCounter, IUseCounter::Uid uid);
		Resource(const Resource& resource);
		Resource(Resource&& resource) = default;
		void operator=(const Resource& resource);
		virtual ~Resource();

		virtual bool isValid() const = 0;

	private:
		IUseCounter* mUseCounter;
		IUseCounter::Uid mUid;
	};
}
