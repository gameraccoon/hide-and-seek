#include "Base/precomp.h"

#include <gtest/gtest.h>

#include "Utils/Functional/ScopeFinalizer.h"

TEST(ScopeFinalizer, CallLambda)
{
	int runCount = 0;
	{
		auto _ = ScopeFinalizer([&runCount](){
			++runCount;
		});
		EXPECT_EQ(0, runCount);
	}
	EXPECT_EQ(1, runCount);
}

TEST(ScopeFinalizer, CallStdFunction)
{
	int runCount = 0;
	{
		auto _ = ScopeFinalizer(std::function([&runCount](){
			++runCount;
		}));
		EXPECT_EQ(0, runCount);
	}
	EXPECT_EQ(1, runCount);
}

static int gLazyEvaluationWrapperTestCallsCount = 0;

static void LazyEvaluatedWrapperTestFn()
{
	++gLazyEvaluationWrapperTestCallsCount;
}

TEST(ScopeFinalizer, CallRawFunction)
{
	gLazyEvaluationWrapperTestCallsCount = 0;
	{
		auto _ = ScopeFinalizer(&LazyEvaluatedWrapperTestFn);
		EXPECT_EQ(0, gLazyEvaluationWrapperTestCallsCount);
	}
	EXPECT_EQ(1, gLazyEvaluationWrapperTestCallsCount);
}

