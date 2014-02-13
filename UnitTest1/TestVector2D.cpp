#include "stdafx.h"
#include "CppUnitTest.h"

#include "../src/Vector2D.cpp"
#include "../src/Rotator.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<Vector2D>(const Vector2D& t) { RETURN_WIDE_STRING(t.x + t.x); }
		}
	}
}

namespace TestVector2D
{
	

	TEST_CLASS(TestVector2D)
	{
	public:
		
		TEST_METHOD(TestVectorCreation)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			Vector2D testVectorB(testVectorA);
		
			Assert::IsTrue(testVectorA.x == 5.2f
					&& testVectorA.y == -0.39f);
			Assert::IsTrue(testVectorB.x == 5.2f
					&& testVectorB.y == -0.39f);
		}

		TEST_METHOD(TestVectorInitializationFromAngle)
		{
			Rotator angle = 0.0f;
			Vector2D testVectorA(angle);

			Assert::AreEqual(testVectorA, Vector2D(1.0, 0.0));

			angle = PI/2;
			testVectorA = Vector2D(angle);

			Assert::AreEqual(testVectorA, Vector2D(0.0, 1.0));
		}

		TEST_METHOD(TestVectorComparison)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			Vector2D testVectorB(testVectorA);
			Vector2D testVectorC(5.199998f, -0.390001f);
			Vector2D testVectorD(5.3f, -0.39f);
		
			Assert::IsTrue(testVectorA == testVectorB);		// A == B
			Assert::AreEqual(testVectorA, testVectorB);	// A == B
			Assert::AreEqual(testVectorA, testVectorC);
			Assert::AreNotEqual(testVectorA, testVectorD); // A != D
		}

		TEST_METHOD(TestVectorProtection)
		{
			Vector2D testVectorA(5.2f, -0.39f);

			Vector2D testVectorB = testVectorA;
			testVectorB.x = 30.0f;

			Assert::AreEqual(testVectorB.x, 30.0f);
			Assert::AreEqual(testVectorA.x, 5.2f);
		}

		TEST_METHOD(TestVectorSize)
		{
			Vector2D testVectorA(5.2f, -0.39f);

			float testVectorSize = testVectorA.size();

			Assert::AreEqual(testVectorSize, 5.2146f, 0.0001f);
		}

		TEST_METHOD(TestVectorQSize)
		{
			Vector2D testVectorA(5.2f, -0.39f);

			float testVectorSize = testVectorA.qSize();

			Assert::AreEqual(testVectorSize, 27.1921f, 0.0001f);
		}

		TEST_METHOD(TestVectorNegation)
		{
			Vector2D testVectorA(5.2f, -0.39f);

			Vector2D testVectorB = -testVectorA;

			Assert::AreEqual(testVectorB, Vector2D(-5.2f, 0.39f));

			testVectorB.y = 70.0f;

			Assert::AreEqual(testVectorA, Vector2D(5.2f, -0.39f));
			Assert::AreEqual(testVectorA.x, 5.2f);
		}

		TEST_METHOD(TestVectorAddition)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			Vector2D testVectorB(16.9f, 803.27f);

			Vector2D testVectorC = testVectorA + testVectorB;
			testVectorA = testVectorB + testVectorC;
			testVectorB += testVectorB;

			Assert::AreEqual(testVectorC, Vector2D(22.1f, 802.88f));
			Assert::AreEqual(testVectorA, Vector2D(39, 1606.15f));
			Assert::AreEqual(testVectorB, Vector2D(33.8f, 1606.54f));
		}

		TEST_METHOD(TestVectorSubstraction)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			Vector2D testVectorB(16.9f, 803.27f);

			Vector2D testVectorC = testVectorA - testVectorB;
			testVectorA = testVectorB - testVectorC;
			testVectorB -= testVectorB;

			Assert::AreEqual(testVectorC, Vector2D(-11.7f, -803.66f));
			Assert::AreEqual(testVectorA, Vector2D(28.6f, 1606.9301f));
			Assert::AreEqual(testVectorB, Vector2D(0.0f, 0.0f));
		}

		TEST_METHOD(TestVectorMultiplication)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			float testScalar = 18.3f;

			Vector2D testVectorB = testVectorA * testScalar;
			testVectorA *= -testScalar;

			Assert::AreEqual(testVectorB, Vector2D(95.16f, -7.137f));
			Assert::AreEqual(testVectorA, Vector2D(-95.16f, 7.137f));
			Assert::AreEqual(testVectorA, -testVectorB);
		}

		TEST_METHOD(TestVectorDivision)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			float testScalar = 18.3f;

			testVectorA /= testScalar;
			Vector2D testVectorB = testVectorA / testVectorA.size();	// B = ort(A);

			Assert::AreEqual(testVectorA, Vector2D(0.2841f, -0.0213f));
			Assert::AreEqual(testVectorB, Vector2D(0.9972f, -0.07476377f));
			Assert::AreEqual(testVectorB.size(), 1.0f);				// ort size is only 1.0
		}

		TEST_METHOD(TestVectorOrt)
		{
			Vector2D testVectorA(5.2f, -0.39f);

			testVectorA = testVectorA.ort();
		
			Assert::AreEqual(testVectorA, Vector2D(0.9972f, -0.07476377f));
			Assert::AreEqual(testVectorA.size(), 1.0f);				// ort size is only 1.0

			testVectorA = Vector2D(0.0f, 0.0f);
			Assert::AreEqual(testVectorA.ort(), Vector2D(0.0f, 0.0f));
		}

		TEST_METHOD(TestVectorGetRotation)
		{
			Vector2D testVectorA(1.0f, 0.0f);
			Vector2D testVectorB(-1.0f, 0.0f);
			Vector2D testVectorC(0.0f, 1.0f);
			Vector2D testVectorD(0.0f, -1.0f);
			Vector2D testVectorE(0.0f, 0.0f);
		
			Assert::AreEqual(testVectorA.rotation().getValue(), 0.0f);
			Assert::AreEqual(testVectorB.rotation().getValue(), PI);
			Assert::AreEqual(testVectorC.rotation().getValue(), PI/2);
			Assert::AreEqual(testVectorD.rotation().getValue(), -PI/2);
			Assert::AreEqual(testVectorE.rotation().getValue(), 0.0f);		//Invalid parameter should not cause errors
		}

		TEST_METHOD(TestVectorDotProduct)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			Vector2D testVectorB(16.9f, 803.27f);

			Assert::AreEqual(DotProduct(testVectorA, testVectorB), -225.3953f);
		}

		TEST_METHOD(TestVectorProject)
		{
			Vector2D oX(1.0f, 0.0f);
			Vector2D oYxFive(0.0f, 5.0f);
		
			Vector2D testVectorA(5.2f, -0.39f);
			Vector2D testVectorB(16.9f, 803.27f);

			// Another (slowlest) way to calc projection vector
			Vector2D projectAB = testVectorA.ort() * testVectorB.size() * cos((testVectorA.rotation() - testVectorB.rotation()).getValue());

			Assert::AreEqual(testVectorA.project(oX), Vector2D(5.2f, 0.0f));
			Assert::AreEqual(testVectorB.project(oYxFive), Vector2D(0.0f, 803.27f));
			Assert::AreEqual(oYxFive.project(oX), Vector2D(0.0f, 0.0f));
			Assert::AreEqual(testVectorA.project(oX) + testVectorA.project(oYxFive), testVectorA);
		
			Assert::AreEqual(testVectorB.project(testVectorA), projectAB);
		}
	};
}