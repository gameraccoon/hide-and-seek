#include "stdafx.h"
#include "CppUnitTest.h"

#include "../src/Processes.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestProcesses
{
	int TestProcessValue1;
	TEST_CLASS(TestProcesses)
	{
		
	class TestProcessA:public Process
	{
	public:
		void Run()
		{
			TestProcessValue1 += 1;
		}

		TestProcessA(int id):Process(id) {}
	};

	class TestProcessB:public Process
	{
	public:
		void Run()
		{
			TestProcessValue1 += 10;
		}

		TestProcessB(int id):Process(id) {}
	};

	class TestProcessC:public Process
	{
	public:
		void Run()
		{
			TestProcessValue1 += 100;
		}

		TestProcessC(int id):Process(id) {}
	};

	public:

		TEST_METHOD(TestProcessID)
		{
			TestProcessA testProcess1(1);
			Assert::AreEqual(testProcess1.GetId(), 1);

			TestProcessA testProcess2(12);
			Assert::AreEqual(testProcess2.GetId(), 12);
		}

		TEST_METHOD(TestProcessManagerAddition)
		{
			ProcessManager procMan;

			procMan.Add(new TestProcessA(0));
			TestProcessValue1 = 0;
			procMan.Run();
			Assert::AreEqual(TestProcessValue1, 1);

			procMan.Add(new TestProcessB(0));
			TestProcessValue1 = 0;
			procMan.Run();
			Assert::AreEqual(TestProcessValue1, 11);
		}

		TEST_METHOD(TestProcessManagerRunEmpty)
		{
			ProcessManager procMan;

			TestProcessValue1 = 0;				// Empty
			procMan.Run();						// Nothing to run
			Assert::AreEqual(TestProcessValue1, 0);

			procMan.Add(new TestProcessA(1));	// A
			procMan.Remove(1);					// Empty
			TestProcessValue1 = 1666;
			procMan.Run();						// Nothing to run
			Assert::AreEqual(TestProcessValue1, 1666);
		}

		TEST_METHOD(TestProcessManagerRemoving)
		{
			ProcessManager procMan;

			// Deleting a Process that been added the first
			procMan.Add(new TestProcessA(1));		// A
			procMan.Add(new TestProcessB(2));		// A + B
			procMan.Remove(1);						// B
			TestProcessValue1 = 0;
			procMan.Run();							// B->Run()
			Assert::AreEqual(TestProcessValue1, 10);

			procMan.Remove(2);						// Empty

			// Deleting a Process that has been added the last
			ProcessManager procMan2;

			procMan2.Add(new TestProcessA(1));		// A
			procMan2.Add(new TestProcessB(2));		// A + B
			procMan2.Remove(2);						// A
			TestProcessValue1 = 0;
			procMan2.Run();							// A->Run()
			Assert::AreEqual(TestProcessValue1, 1);

			procMan2.Remove(1);						// Empty

			// Deleting a process that has been added not the first and not the last
			ProcessManager procMan3;

			procMan3.Add(new TestProcessA(1));		// A
			procMan3.Add(new TestProcessB(2));		// A + B + C
			procMan3.Add(new TestProcessC(3));		// A + B + C
			procMan3.Remove(2);						// A + C
			TestProcessValue1 = 0;
			procMan3.Run();							// A->Run(); C->Run()
			Assert::AreEqual(TestProcessValue1, 101);

			// Remove next
			procMan3.Remove(1);						// C
			TestProcessValue1 = 0;
			procMan3.Run();							// C->Run()
			Assert::AreEqual(TestProcessValue1, 100);

			// Remove last one
			procMan3.Remove(3);						// Empty
			TestProcessValue1 = 0;
			procMan3.Run();							// Nothing to run
			Assert::AreEqual(TestProcessValue1, 0);
		}

		TEST_METHOD(TestProcessManagerDuplication)
		{
			ProcessManager procMan1;

			procMan1.Add(new TestProcessA(1));	// A
			procMan1.Add(new TestProcessA(1));	// A + A
			TestProcessValue1 = 0;
			procMan1.Run();						// A->Run(); A->Run()
			Assert::AreEqual(TestProcessValue1, 2);

			ProcessManager procMan2;

			procMan2.Add(new TestProcessA(1));	// A
			procMan2.Add(new TestProcessB(2));	// A + B
			procMan2.Add(new TestProcessC(1));	// A(1) + B(2) + C(1)
			TestProcessValue1 = 0;
			procMan2.Run();						// A->Run(); B->Run(); C->Run()
			Assert::AreEqual(TestProcessValue1, 111);

			procMan2.Remove(1);					// B // A and C was deleted
			TestProcessValue1 = 0;
			procMan2.Run();						// B->Run()
			Assert::AreEqual(TestProcessValue1, 10);
		}
	};
}