#include "Base/precomp.h"

#include <gtest/gtest.h>

#include "Utils/LazyEvaluatedWrapper.h"

TEST(LazyEvaluatedWrapper, CallLambda)
{
	int runCount = 0;
	LazyEvaluated lazyEvaluatedLambda([&runCount](){
		++runCount;
		return 8;
	});

	EXPECT_EQ(0, runCount);
	EXPECT_EQ(8, lazyEvaluatedLambda());
	EXPECT_EQ(1, runCount);
	EXPECT_EQ(8, lazyEvaluatedLambda());
	EXPECT_EQ(1, runCount);
}

TEST(LazyEvaluatedWrapper, CallStdFunction)
{
	int runCount = 0;
	LazyEvaluated lazyEvaluatedStdFunction(std::function([&runCount](){
		++runCount;
		return 9;
	}));

	EXPECT_EQ(0, runCount);
	EXPECT_EQ(9, lazyEvaluatedStdFunction());
	EXPECT_EQ(1, runCount);
	EXPECT_EQ(9, lazyEvaluatedStdFunction());
	EXPECT_EQ(1, runCount);
}

static int gLazyEvaluationWrapperTestInt = 0;

static int LazyEvaluatedWrapperTestFn()
{
	++gLazyEvaluationWrapperTestInt;
	return 10;
}

TEST(LazyEvaluatedWrapper, CallRawFunctionWOArguments)
{
	gLazyEvaluationWrapperTestInt = 0;
	LazyEvaluated lazyEvaluatedFunction(&LazyEvaluatedWrapperTestFn);

	EXPECT_EQ(0, gLazyEvaluationWrapperTestInt);
	EXPECT_EQ(10, lazyEvaluatedFunction());
	EXPECT_EQ(1, gLazyEvaluationWrapperTestInt);
	EXPECT_EQ(10, lazyEvaluatedFunction());
	EXPECT_EQ(1, gLazyEvaluationWrapperTestInt);
}
