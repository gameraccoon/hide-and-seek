#include <cfixcc.h>

#include "..\src\Vector2D.h"

class testVector : public cfixcc::TestFixture
{
private:

public:
	void testVectorCreation()
	{
		Vector2D testVectorA(5.2f, -0.39f);
		Vector2D testVectorB(testVectorA);
		
		CFIXCC_ASSERT(testVectorA.X == 5.2f
				&& testVectorA.Y == -0.39f);
		CFIXCC_ASSERT(testVectorB.X == 5.2f
				&& testVectorB.Y == -0.39f);
	}

	void testVectorComparison()
	{
		Vector2D testVectorA(5.2f, -0.39f);
		Vector2D testVectorB(testVectorA);
		Vector2D testVectorC(5.199998f, -0.390001f);
		Vector2D testVectorD(5.3f, -0.39f);
		
		CFIXCC_ASSERT(testVectorA == testVectorB);		// A == B
		CFIXCC_ASSERT_EQUALS(testVectorA, testVectorB);	// A == B
		CFIXCC_ASSERT_EQUALS(testVectorA, testVectorC);
		CFIXCC_ASSERT_NOT_EQUALS(testVectorA, testVectorD); // A != D
	}

	void testVectorProtection()
	{
		Vector2D testVectorA(5.2f, -0.39f);

		Vector2D testVectorB = testVectorA;
		testVectorB.X = 30.0f;
		CFIXCC_ASSERT_EQUALS(testVectorB.X, 30.0f);
		CFIXCC_ASSERT_EQUALS(testVectorA.X, 5.2f);
	}

	void testVectorSize()
	{
		Vector2D testVectorA(5.2f, -0.39f);

		float testVectorSize = testVectorA.Size();
		CFIXCC_ASSERT_EQUALS(testVectorSize, 5.2146f);
	}

	void testVectorNegation()
	{
		Vector2D testVectorA(5.2f, -0.39f);

		Vector2D testVectorB = -testVectorA;
		CFIXCC_ASSERT_EQUALS(testVectorB, Vector2D(-5.2f, 0.39f));

		testVectorB.Y = 70.0f;
		CFIXCC_ASSERT_EQUALS(testVectorA, Vector2D(5.2f, -0.39f));
		CFIXCC_ASSERT_EQUALS(testVectorA.X, 5.2f);
	}

	void testVectorAddition()
	{
		Vector2D testVectorA(5.2f, -0.39f);
		Vector2D testVectorB(16.9f, 803.27f);

		Vector2D testVectorC = testVectorA + testVectorB;
		testVectorA = testVectorB + testVectorC;
		testVectorB += testVectorB;

		CFIXCC_ASSERT_EQUALS(testVectorC, Vector2D(22.1f, 802.88f));
		CFIXCC_ASSERT_EQUALS(testVectorA, Vector2D(39, 1606.15f));
		CFIXCC_ASSERT_EQUALS(testVectorB, Vector2D(33.8f, 1606.54f));
	}

	void testVectorSubstraction()
	{
		Vector2D testVectorA(5.2f, -0.39f);
		Vector2D testVectorB(16.9f, 803.27f);

		Vector2D testVectorC = testVectorA - testVectorB;
		testVectorA = testVectorB - testVectorC;
		testVectorB -= testVectorB;

		CFIXCC_ASSERT_EQUALS(testVectorC, Vector2D(-11.7f, -803.66f));
		CFIXCC_ASSERT_EQUALS(testVectorA, Vector2D(28.6f, 1606.9301f));
		CFIXCC_ASSERT_EQUALS(testVectorB, Vector2D(0.0f, 0.0f));
	}

	void testVectorMultiplication()
	{
		Vector2D testVectorA(5.2f, -0.39f);
		float testScalar = 18.3f;

		Vector2D testVectorB = testVectorA * testScalar;
		testVectorA *= -testScalar;

		CFIXCC_ASSERT_EQUALS(testVectorB, Vector2D(95.16f, -7.137f));
		CFIXCC_ASSERT_EQUALS(testVectorA, Vector2D(-95.16f, 7.137f));
		CFIXCC_ASSERT_EQUALS(testVectorA, -testVectorB);
	}

	void testVectorDivision()
	{
		Vector2D testVectorA(5.2f, -0.39f);
		float testScalar = 18.3f;

		testVectorA /= testScalar;
		Vector2D testVectorB = testVectorA / testVectorA.Size();	// B = ort(A);

		CFIXCC_ASSERT_EQUALS(testVectorA, Vector2D(0.2841f, -0.0213f));
		CFIXCC_ASSERT_EQUALS(testVectorB, Vector2D(0.9972f, -0.07476377f));
		CFIXCC_ASSERT_EQUALS(testVectorB.Size(), 1.0f);				// ort size is only 1.0
	}

	void testVectorOrt()
	{
		Vector2D testVectorA(5.2f, -0.39f);

		testVectorA = testVectorA.Ort();
		
		CFIXCC_ASSERT_EQUALS(testVectorA, Vector2D(0.9972f, -0.07476377f));
		CFIXCC_ASSERT_EQUALS(testVectorA.Size(), 1.0f);				// ort size is only 1.0

		testVectorA = Vector2D(0.0f, 0.0f);
		CFIXCC_ASSERT_EQUALS(testVectorA.Ort(), Vector2D(0.0f, 0.0f));
	}
};

CFIXCC_BEGIN_CLASS(testVector)
	CFIXCC_METHOD(testVectorCreation)
	CFIXCC_METHOD(testVectorComparison)
	CFIXCC_METHOD(testVectorProtection)
	CFIXCC_METHOD(testVectorSize)
	CFIXCC_METHOD(testVectorNegation)
	CFIXCC_METHOD(testVectorAddition)
	CFIXCC_METHOD(testVectorSubstraction)
	CFIXCC_METHOD(testVectorMultiplication)
	CFIXCC_METHOD(testVectorDivision)
	CFIXCC_METHOD(testVectorOrt)
CFIXCC_END_CLASS()

