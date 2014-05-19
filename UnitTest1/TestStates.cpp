#include "stdafx.h"
#include "CppUnitTest.h"

#include "../src/Engine/Ai/AiState.cpp"
#include "../src/Engine/Ai/AiStatesStack.cpp"
#include "../src/Engine/Actors/SoundVolume.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestStates
{	
	int TestValueForTestState;
	TEST_CLASS(TestStates)
	{
		class TestStateA:public AiState
		{
		public:
			TestStateA(World *world, IBody *body, Role *role) : AiState(world, body, role)
			{

			}
			~TestStateA()
			{
			}

			virtual void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse) override final
			{
				TestValueForTestState = 1;
			}

			virtual void onSeeEnemy(IActor *enemy) override final
			{
			}

			virtual void onHearNoise(SoundVolume *sound) override final
			{
			}
		};

		class TestStateB:public AiState
		{
		public:
			TestStateB(World *world, IBody *body, Role *role) : AiState(world, body, role)
			{

			}

			~TestStateB()
			{
			}

			virtual void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse) override final
			{
				TestValueForTestState = 2;
			}

			virtual void onSeeEnemy(IActor *enemy) override final
			{
			}

			virtual void onHearNoise(SoundVolume *sound) override final
			{
			}
		};

		class TestStateC:public AiState
		{
		public:
			TestStateC(World *world, IBody *body, Role *role) : AiState(world, body, role)
			{

			}

			~TestStateC()
			{
			}

			virtual void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse) override final
			{
				TestValueForTestState = 3;
			}

			virtual void onSeeEnemy(IActor *enemy) override final
			{
			}

			virtual void onHearNoise(SoundVolume *sound) override final
			{
			}
		};
	public:
		TEST_METHOD(TestStatesStackOneState)
		{
			// Test processing statestack with one state
			TestValueForTestState = 0;
			AiStatesStack Stack;
			Stack.push(new TestStateA(nullptr, nullptr, nullptr));
			Stack.onTakeDamage(nullptr, 1.f, ZERO_VECTOR);
			Assert::AreEqual(1, TestValueForTestState);
		}

		TEST_METHOD(TestStatesStackMultiplyState)
		{
			// Test processing statestack with one state
			TestValueForTestState = 0;
			AiStatesStack Stack;
			Stack.push(new TestStateA(nullptr, nullptr, nullptr));	// A <-
			Stack.push(new TestStateB(nullptr, nullptr, nullptr));	// A B <-
			Stack.onTakeDamage(nullptr, 1.f, ZERO_VECTOR);			// Run B->Process()
			Assert::AreEqual(2, TestValueForTestState);
			TestValueForTestState = 0;
			Stack.push(new TestStateC(nullptr, nullptr, nullptr));	// A B C <-
			Stack.onTakeDamage(nullptr, 1.f, ZERO_VECTOR);			// Run C->Process()
			Assert::AreEqual(3, TestValueForTestState);
			TestValueForTestState = 0;
			Stack.pop();					// A B <-
			Stack.onTakeDamage(nullptr, 1.f, ZERO_VECTOR);			// Run B->Process()
			Assert::AreEqual(2, TestValueForTestState);
			TestValueForTestState = 0;
			Stack.pop();					// A <-
			Stack.onTakeDamage(nullptr, 1.f, ZERO_VECTOR);			// Run A->Process()
			Assert::AreEqual(1, TestValueForTestState);
			TestValueForTestState = 0;
			Stack.pop();					// Nothing in stack
		}
	};
}