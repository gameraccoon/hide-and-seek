#include "Graphics/Resource.h"

Resource::Resource(IUseCounter* useCounter, IUseCounter::Uid uid)
	: mUseCounter(useCounter)
	, mUid(uid)
{
	mUseCounter->beginUse(uid);
}

Resource::Resource(const Resource& resource)
	: mUid(resource.mUid)
	, mUseCounter(resource.mUseCounter)
{
	mUseCounter->beginUse(resource.mUid);
}

void Resource::operator=(const Resource& resource)
{
	mUid = resource.mUid;
	mUseCounter = resource.mUseCounter;

	mUseCounter->beginUse(mUid);
}

Resource::~Resource()
{
	mUseCounter->endUse(mUid);
}

Resource::Base::Base(IUseCounter::Uid uid, Destructor destructor)
	: uid(uid)
	, destructor(destructor)
{
}

Resource::Base::~Base()
{
	if (destructor)
	{
		destructor(this);
	}
}
