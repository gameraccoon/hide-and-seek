#include <gtest/gtest.h>

#include "Base/String/StringID.h"

#include "UnitTests/TestAssertHelper.h"

TEST(StringID, TestHashingSanity)
{
	constexpr StringID valA1 = STR_TO_ID("testA");
	constexpr StringID valB1 = STR_TO_ID("testB");
	// these also should be calculated at compile time
	StringID valA2 = STR_TO_ID("testA");
	StringID valB2 = STR_TO_ID("testB");
	std::string valA3 = "testA";
	std::string valB3 = "testB";

	EXPECT_EQ(valA1, valA2);
	EXPECT_EQ(valA2, STR_TO_ID(valA3));
	EXPECT_NE(valA1, valB1);
	EXPECT_NE(valA2, valB2);
	EXPECT_NE(valA2, STR_TO_ID(valB3));
}

TEST(StringID, TestIDToString)
{
	// compile time generated StringID
	StringID testC = STR_TO_ID("testC");
	EXPECT_EQ(std::string("testC"), ID_TO_STR(testC));

	// runtime generated StringID
	std::string testDStr = "testD";
	StringID testD = STR_TO_ID(testDStr);
	EXPECT_EQ(testDStr, ID_TO_STR(testD));
}
