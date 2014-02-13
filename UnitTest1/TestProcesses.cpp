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
		void run()
		{
			TestProcessValue1 += 1;
		}

		TestProcessA(int id):Process(id) {}
	};

	class TestProcessB:public Process
	{
	public:
		void run()
		{
			TestProcessValue1 += 10;
		}

		TestProcessB(int id):Process(id) {}
	};

	class TestProcessC:public Process
	{
	public:
		void run()
		{
			TestProcessValue1 += 100;
		}

		TestProcessC(int id):Process(id) {}
	};

	public:

		TEST_METHOD(TestProcessID)
		{
			TestProcessA testProcess1(1);
			Assert::AreEqual(testProcess1.getId(), 1);

			TestProcessA testProcess2(12);
			Assert::AreEqual(testProcess2.getId(), 12);
		}

		TEST_METHOD(TestProcessManagerAddition)
		{
			ProcessManager procMan;

			procMan.add(new TestProcessA(0));
			TestProcessValue1 = 0;
			procMan.run();
			Assert::AreEqual(TestProcessValue1, 1);

			procMan.add(new TestProcessB(0));
			TestProcessValue1 = 0;
			procMan.run();
			Assert::AreEqual(TestProcessValue1, 11);
		}

		TEST_METHOD(TestProcessManagerRunEmpty)
		{
			ProcessManager procMan;

			TestProcessValue1 = 0;				// Empty
			procMan.run();						// Nothing to run
			Assert::AreEqual(TestProcessValue1, 0);

			procMan.add(new TestProcessA(1));	// A
			procMan.remove(1);					// Empty
			TestProcessValue1 = 1666;
			procMan.run();						// Nothing to run
			Assert::AreEqual(TestProcessValue1, 1666);
		}

		TEST_METHOD(TestProcessManagerRemoving)
		{
			ProcessManager procMan;

			// Deleting a Process that been added the first
			procMan.add(new TestProcessA(1));		// A
			procMan.add(new TestProcessB(2));		// A + B
			procMan.remove(1);						// B
			TestProcessValue1 = 0;
			procMan.run();							// B->Run()
			Assert::AreEqual(TestProcessValue1, 10);

			procMan.remove(2);						// Empty

			// Deleting a Process that has been added the last
			ProcessManager procMan2;

			procMan2.add(new TestProcessA(1));		// A
			procMan2.add(new TestProcessB(2));		// A + B
			procMan2.remove(2);						// A
			TestProcessValue1 = 0;
			procMan2.run();							// A->Run()
			Assert::AreEqual(TestProcessValue1, 1);

			procMan2.remove(1);						// Empty

			// Deleting a process that has been added not the first and not the last
			ProcessManager procMan3;

			procMan3.add(new TestProcessA(1));		// A
			procMan3.add(new TestProcessB(2));		// A + B + C
			procMan3.add(new TestProcessC(3));		// A + B + C
			procMan3.remove(2);						// A + C
			TestProcessValue1 = 0;
			procMan3.run();							// A->Run(); C->Run()
			Assert::AreEqual(TestProcessValue1, 101);

			// Remove next
			procMan3.remove(1);						// C
			TestProcessValue1 = 0;
			procMan3.run();							// C->Run()
			Assert::AreEqual(TestProcessValue1, 100);

			// Remove last one
			procMan3.remove(3);						// Empty
			TestProcessValue1 = 0;
			procMan3.run();							// Nothing to run
			Assert::AreEqual(TestProcessValue1, 0);
		}

		TEST_METHOD(TestProcessManagerDuplication)
		{
			ProcessManager procMan1;

			procMan1.add(new TestProcessA(1));	// A
			procMan1.add(new TestProcessA(1));	// A + A
			TestProcessValue1 = 0;
			procMan1.run();						// A->Run(); A->Run()
			Assert::AreEqual(TestProcessValue1, 2);

			ProcessManager procMan2;

			procMan2.add(new TestProcessA(1));	// A
			procMan2.add(new TestProcessB(2));	// A + B
			procMan2.add(new TestProcessC(1));	// A(1) + B(2) + C(1)
			TestProcessValue1 = 0;
			procMan2.run();						// A->Run(); B->Run(); C->Run()
			Assert::AreEqual(TestProcessValue1, 111);

			procMan2.remove(1);					// B // A and C was deleted
			TestProcessValue1 = 0;
			procMan2.run();						// B->Run()
			Assert::AreEqual(TestProcessValue1, 10);
		}
	};
}