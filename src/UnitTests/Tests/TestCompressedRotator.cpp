#include <gtest/gtest.h>

#include "Base/Types/BasicTypes.h"

#include "GameData/Core/Vector2D.h"
#include "GameData/Core/CompressedRotator.h"

TEST(CompressedRotator, Value)
{
	Rotator zeroRotator{0};
	Rotator minusPiRotator{-PI};
	Rotator piRotator{PI};

	EXPECT_NEAR(zeroRotator.getValue(), CompressedRotator<u16>{zeroRotator}.decompress().getValue(), 0.001);
	EXPECT_NEAR(minusPiRotator.getValue(), CompressedRotator<u16>{minusPiRotator}.decompress().getValue(), 0.001);
	EXPECT_NEAR(piRotator.getValue(), CompressedRotator<u16>{piRotator}.decompress().getValue(), 0.001);

	static_assert(std::is_pod<CompressedRotator<u16>>(), "CompressedRotator should be POD type");
}

TEST(CompressedRotator, Static)
{
	Rotator zeroRotator{0};
	Rotator minusPiRotator{-PI};
	Rotator piRotator{PI};

	u16 zeroInt = CompressedRotator<u16>::Compress(zeroRotator, 16);
	u16 minusPiInt = CompressedRotator<u16>::Compress(minusPiRotator, 16);
	u16 piInt = CompressedRotator<u16>::Compress(piRotator, 16);

	EXPECT_EQ(0u, zeroInt);

	EXPECT_NEAR(zeroRotator.getValue(), CompressedRotator<u16>::Decompress(zeroInt).getValue(), 0.001);
	EXPECT_NEAR(minusPiRotator.getValue(), CompressedRotator<u16>::Decompress(minusPiInt).getValue(), 0.001);
	EXPECT_NEAR(piRotator.getValue(), CompressedRotator<u16>::Decompress(piInt).getValue(), 0.001);
}
