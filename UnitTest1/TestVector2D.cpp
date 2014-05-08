#include "stdafx.h"
#include "CppUnitTest.h"

#include "../src/Engine/Core/Vector2D.cpp"
#include "../src/Engine/Core/Rotator.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<Vector2D>(const Vector2D& t) { RETURN_WIDE_STRING(t.x); }
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

			Assert::AreEqual(Vector2D(1.0, 0.0), testVectorA);

			angle = PI/2;
			testVectorA = Vector2D(angle);

			Assert::AreEqual(Vector2D(0.0, 1.0), testVectorA);
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

			Assert::AreEqual(30.0f, testVectorB.x);
			Assert::AreEqual(5.2f, testVectorA.x);
		}

		TEST_METHOD(TestVectorSize)
		{
			Vector2D testVectorA(5.2f, -0.39f);

			float testVectorSize = testVectorA.size();

			Assert::AreEqual(5.2146f, testVectorSize, 0.0001f);
		}

		TEST_METHOD(TestVectorQSize)
		{
			Vector2D testVectorA(5.2f, -0.39f);

			float testVectorSize = testVectorA.qSize();

			Assert::AreEqual(27.1921f, testVectorSize, 0.0001f);
		}

		TEST_METHOD(TestVectorNegation)
		{
			Vector2D testVectorA(5.2f, -0.39f);

			Vector2D testVectorB = -testVectorA;

			Assert::AreEqual(Vector2D(-5.2f, 0.39f), testVectorB);

			testVectorB.y = 70.0f;

			Assert::AreEqual(Vector2D(5.2f, -0.39f), testVectorA);
			Assert::AreEqual(5.2f, testVectorA.x);
		}

		TEST_METHOD(TestVectorAddition)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			Vector2D testVectorB(16.9f, 803.27f);

			Vector2D testVectorC = testVectorA + testVectorB;
			testVectorA = testVectorB + testVectorC;
			testVectorB += testVectorB;

			Assert::AreEqual(Vector2D(22.1f, 802.88f), testVectorC);
			Assert::AreEqual(Vector2D(39, 1606.15f), testVectorA);
			Assert::AreEqual(Vector2D(33.8f, 1606.54f), testVectorB);
		}

		TEST_METHOD(TestVectorSubstraction)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			Vector2D testVectorB(16.9f, 803.27f);

			Vector2D testVectorC = testVectorA - testVectorB;
			testVectorA = testVectorB - testVectorC;
			testVectorB -= testVectorB;

			Assert::AreEqual(Vector2D(-11.7f, -803.66f), testVectorC);
			Assert::AreEqual(Vector2D(28.6f, 1606.9301f), testVectorA);
			Assert::AreEqual(Vector2D(0.0f, 0.0f), testVectorB);
		}

		TEST_METHOD(TestVectorMultiplication)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			float testScalar = 18.3f;

			Vector2D testVectorB = testVectorA * testScalar;
			testVectorA *= -testScalar;

			Assert::AreEqual(Vector2D(95.16f, -7.137f), testVectorB);
			Assert::AreEqual(Vector2D(-95.16f, 7.137f), testVectorA);
			Assert::AreEqual(testVectorA, -testVectorB);
		}

		TEST_METHOD(TestVectorDivision)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			float testScalar = 18.3f;

			testVectorA /= testScalar;
			Vector2D testVectorB = testVectorA / testVectorA.size();	// B = ort(A);

			Assert::AreEqual(Vector2D(0.2841f, -0.0213f), testVectorA);
			Assert::AreEqual(Vector2D(0.9972f, -0.07476377f), testVectorB);
			Assert::AreEqual(1.0f, testVectorB.size());					// ort size is only 1.0
		}

		TEST_METHOD(TestVectorOrt)
		{
			Vector2D testVectorA(5.2f, -0.39f);

			testVectorA = testVectorA.ort();
		
			Assert::AreEqual(Vector2D(0.9972f, -0.07476377f), testVectorA);
			Assert::AreEqual(1.0f, testVectorA.size());				// ort size is only 1.0

			testVectorA = Vector2D(0.0f, 0.0f);
			Assert::AreEqual(Vector2D(0.0f, 0.0f), testVectorA.ort());
		}

		TEST_METHOD(TestVectorGetRotation)
		{
			Vector2D testVectorA(1.0f, 0.0f);
			Vector2D testVectorB(-1.0f, 0.0f);
			Vector2D testVectorC(0.0f, 1.0f);
			Vector2D testVectorD(0.0f, -1.0f);
			Vector2D testVectorE(0.0f, 0.0f);
		
			Assert::AreEqual(0.0f, testVectorA.rotation().getValue());
			Assert::AreEqual(PI, testVectorB.rotation().getValue());
			Assert::AreEqual(PI/2, testVectorC.rotation().getValue());
			Assert::AreEqual(-PI/2, testVectorD.rotation().getValue());
			Assert::AreEqual(0.0f, testVectorE.rotation().getValue());		//Invalid parameter should not cause errors
		}

		TEST_METHOD(TestVectorDotProduct)
		{
			Vector2D testVectorA(5.2f, -0.39f);
			Vector2D testVectorB(16.9f, 803.27f);

			Assert::AreEqual(-225.3953f, DotProduct(testVectorA, testVectorB));
		}

		TEST_METHOD(TestVectorProject)
		{
			Vector2D oX(1.0f, 0.0f);
			Vector2D oYxFive(0.0f, 5.0f);
		
			Vector2D testVectorA(5.2f, -0.39f);
			Vector2D testVectorB(16.9f, 803.27f);

			// Another (slowlest) way to calc projection vector
			Vector2D projectAB = testVectorA.ort() * testVectorB.size() * cos((testVectorA.rotation() - testVectorB.rotation()).getValue());

			Assert::AreEqual(Vector2D(5.2f, 0.0f), testVectorA.project(oX));
			Assert::AreEqual(Vector2D(0.0f, 803.27f), testVectorB.project(oYxFive));
			Assert::AreEqual(Vector2D(0.0f, 0.0f), oYxFive.project(oX));
			Assert::AreEqual(testVectorA.project(oX) + testVectorA.project(oYxFive), testVectorA);
		
			Assert::AreEqual(projectAB, testVectorB.project(testVectorA));
		}
	};
}