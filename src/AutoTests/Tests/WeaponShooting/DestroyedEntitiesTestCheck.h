#pragma once

#include "AutoTests/TestChecklist.h"

class DestroyedEntitiesTestCheck : public TestCheck
{
public:
	DestroyedEntitiesTestCheck(int expectedDestroyedEntities)
		: mExpectedDestroyedEntities(expectedDestroyedEntities)
	{}

	bool isPassed() const override { return mDestroyedEntities == mExpectedDestroyedEntities; };
	std::string describe() const override
	{
		return FormatString("Destroyed entities: %d, expected %d.", mDestroyedEntities, mExpectedDestroyedEntities);
	}

	void addDestroyedEntities(int count) { mDestroyedEntities += count; }

private:
	int mDestroyedEntities = 0;
	int mExpectedDestroyedEntities;
};

