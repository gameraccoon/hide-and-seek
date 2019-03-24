#include <gtest/gtest.h>

#include <Core/Vector2D.h>
#include <Core/Rotator.h>


TEST(Vector2D, Creation)
{
	Vector2D testVectorA(5.2f, -0.39f);
	Vector2D testVectorB(testVectorA);
		
	EXPECT_FLOAT_EQ(testVectorA.x, 5.2f);
	EXPECT_FLOAT_EQ(testVectorA.y, -0.39f);
	EXPECT_FLOAT_EQ(testVectorB.x, 5.2f);
	EXPECT_FLOAT_EQ(testVectorB.y, -0.39f);
}

TEST(Vector2D, InitializationFromAngle)
{
	Rotator angle = 0.0f;
	Vector2D testVectorA(angle);

	EXPECT_EQ(Vector2D(1.0, 0.0), testVectorA);

	angle = PI/2;
	testVectorA = Vector2D(angle);

	EXPECT_EQ(Vector2D(0.0, 1.0), testVectorA);
}

TEST(Vector2D, Comparison)
{
	Vector2D testVectorA(5.2f, -0.39f);
	Vector2D testVectorB(testVectorA);
	Vector2D testVectorC(5.199998f, -0.390001f);
	Vector2D testVectorD(5.3f, -0.39f);
		
	EXPECT_TRUE(testVectorA == testVectorB);		// A == B
	EXPECT_EQ(testVectorA, testVectorB);	// A == B
	EXPECT_EQ(testVectorA, testVectorC);
	EXPECT_NE(testVectorA, testVectorD); // A != D
}

TEST(Vector2D, Protection)
{
	Vector2D testVectorA(5.2f, -0.39f);

	Vector2D testVectorB = testVectorA;
	testVectorB.x = 30.0f;

	EXPECT_FLOAT_EQ(30.0f, testVectorB.x);
	EXPECT_FLOAT_EQ(5.2f, testVectorA.x);
}

TEST(Vector2D, Size)
{
	Vector2D testVectorA(5.2f, -0.39f);

	float testVectorSize = testVectorA.size();

	EXPECT_NEAR(5.2146f, testVectorSize, 0.00001);
}

TEST(Vector2D, QSize)
{
	Vector2D testVectorA(5.2f, -0.39f);

	float testVectorSize = testVectorA.qSize();

	EXPECT_FLOAT_EQ(27.1921f, testVectorSize);
}

TEST(Vector2D, Negation)
{
	Vector2D testVectorA(5.2f, -0.39f);

	Vector2D testVectorB = -testVectorA;

	EXPECT_EQ(Vector2D(-5.2f, 0.39f), testVectorB);

	testVectorB.y = 70.0f;

	EXPECT_EQ(Vector2D(5.2f, -0.39f), testVectorA);
	EXPECT_FLOAT_EQ(5.2f, testVectorA.x);
}

TEST(Vector2D, Addition)
{
	Vector2D testVectorA(5.2f, -0.39f);
	Vector2D testVectorB(16.9f, 803.27f);

	Vector2D testVectorC = testVectorA + testVectorB;
	testVectorA = testVectorB + testVectorC;
	testVectorB += testVectorB;

	EXPECT_EQ(Vector2D(22.1f, 802.88f), testVectorC);
	EXPECT_EQ(Vector2D(39, 1606.15f), testVectorA);
	EXPECT_EQ(Vector2D(33.8f, 1606.54f), testVectorB);
}

TEST(Vector2D, Substraction)
{
	Vector2D testVectorA(5.2f, -0.39f);
	Vector2D testVectorB(16.9f, 803.27f);

	Vector2D testVectorC = testVectorA - testVectorB;
	testVectorA = testVectorB - testVectorC;
	testVectorB -= testVectorB;

	EXPECT_EQ(Vector2D(-11.7f, -803.66f), testVectorC);
	EXPECT_EQ(Vector2D(28.6f, 1606.9301f), testVectorA);
	EXPECT_EQ(Vector2D(0.0f, 0.0f), testVectorB);
}

TEST(Vector2D, Multiplication)
{
	Vector2D testVectorA(5.2f, -0.39f);
	float testScalar = 18.3f;

	Vector2D testVectorB = testVectorA * testScalar;
	testVectorA *= -testScalar;

	EXPECT_EQ(Vector2D(95.16f, -7.137f), testVectorB);
	EXPECT_EQ(Vector2D(-95.16f, 7.137f), testVectorA);
	EXPECT_EQ(testVectorA, -testVectorB);
}

TEST(Vector2D, Division)
{
	Vector2D testVectorA(5.2f, -0.39f);
	float testScalar = 18.3f;

	testVectorA /= testScalar;
	Vector2D testVectorB = testVectorA / testVectorA.size();	// B = ort(A);

	EXPECT_EQ(Vector2D(0.2841f, -0.0213f), testVectorA);
	EXPECT_EQ(Vector2D(0.9972f, -0.07476377f), testVectorB);
	EXPECT_FLOAT_EQ(1.0f, testVectorB.size());					// ort size is only 1.0
}

TEST(Vector2D, Ort)
{
	Vector2D testVectorA(5.2f, -0.39f);

	testVectorA = testVectorA.ort();
		
	EXPECT_EQ(Vector2D(0.9972f, -0.07476377f), testVectorA);
	EXPECT_FLOAT_EQ(1.0f, testVectorA.size());				// ort size is only 1.0

	testVectorA = Vector2D(0.0f, 0.0f);
	EXPECT_EQ(Vector2D(0.0f, 0.0f), testVectorA.ort());
}

TEST(Vector2D, GetRotation)
{
	Vector2D testVectorA(1.0f, 0.0f);
	Vector2D testVectorB(-1.0f, 0.0f);
	Vector2D testVectorC(0.0f, 1.0f);
	Vector2D testVectorD(0.0f, -1.0f);
	Vector2D testVectorE(0.0f, 0.0f);
		
	EXPECT_FLOAT_EQ(0.0f, testVectorA.rotation().getValue());
	EXPECT_FLOAT_EQ(PI, testVectorB.rotation().getValue());
	EXPECT_FLOAT_EQ(PI/2, testVectorC.rotation().getValue());
	EXPECT_FLOAT_EQ(-PI/2, testVectorD.rotation().getValue());
	EXPECT_FLOAT_EQ(0.0f, testVectorE.rotation().getValue());		//Invalid parameter should not cause errors
}

TEST(Vector2D, DotProduct)
{
	Vector2D testVectorA(5.2f, -0.39f);
	Vector2D testVectorB(16.9f, 803.27f);

	EXPECT_EQ(-225.3953f, DotProduct(testVectorA, testVectorB));
}

TEST(Vector2D, Project)
{
	Vector2D oX(1.0f, 0.0f);
	Vector2D oYxFive(0.0f, 5.0f);
		
	Vector2D testVectorA(5.2f, -0.39f);
	Vector2D testVectorB(16.9f, 803.27f);

	// Another (slowlest) way to calc projection vector
	Vector2D projectAB = testVectorA.ort() * testVectorB.size() * cos((testVectorA.rotation() - testVectorB.rotation()).getValue());

	EXPECT_EQ(Vector2D(5.2f, 0.0f), testVectorA.project(oX));
	EXPECT_EQ(Vector2D(0.0f, 803.27f), testVectorB.project(oYxFive));
	EXPECT_EQ(Vector2D(0.0f, 0.0f), oYxFive.project(oX));
	EXPECT_EQ(testVectorA.project(oX) + testVectorA.project(oYxFive), testVectorA);
		
	EXPECT_EQ(projectAB, testVectorB.project(testVectorA));
}
