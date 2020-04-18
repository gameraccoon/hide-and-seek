#include "Base/precomp.h"

#include "Base/Debug/MemoryLeakDetection.h"

#ifdef DETECT_MEMORY_LEAKS

LeakDetectorClass::~LeakDetectorClass()
{
	printIssues();

	freeAllocatedList();
	freeFreedList();
	freeIssuesList(memoryIssuesFirst);
}

void LeakDetectorClass::AddAllocated(void* p, size_t size, const char* file, int line) noexcept
{
	std::lock_guard<std::recursive_mutex> g(mutex);

	void* nextItemAddr = malloc(sizeof(AllocatedPtr));
	AllocatedPtr* nextItem = new (nextItemAddr) AllocatedPtr(p, size, file, line, allocatedList);
	allocatedList = nextItem;

	clearFreedPointer(p);
}

void LeakDetectorClass::AddDeallocated(void* p, const char* file, int line) noexcept
{
	std::lock_guard<std::recursive_mutex> g(mutex);

	if (p == nullptr)
	{
		return;
	}

	if (allocatedList == nullptr)
	{
		exploreNotAllocatedPtr(p, file, line);
		return;
	}

	if (allocatedList->pointer == p)
	{
		AllocatedPtr* item = allocatedList;
		allocatedList = allocatedList->next;
		freeAllocatedItem(item);
		return;
	}

	AllocatedPtr* item = allocatedList;

	while (item->next != nullptr)
	{
		AllocatedPtr* next = item->next;
		if (next->pointer == p)
		{
			item->next = next->next;
			freeAllocatedItem(next);
			return;
		}
		item = next;
	}

	exploreNotAllocatedPtr(p, file, line);
}

void LeakDetectorClass::freeAllocatedItem(LeakDetectorClass::AllocatedPtr* item)
{
	item->next = freedList;
	freedList = item;
}

void LeakDetectorClass::clearFreedPointer(void* p)
{
	if (freedList != nullptr)
	{
		AllocatedPtr* item = freedList;

		if (freedList->next == p)
		{
			freedList = item->next;
			item->~AllocatedPtr();
			free(item);
			return;
		}

		while (item->next != nullptr)
		{
			AllocatedPtr* next = item->next;
			if (next->pointer == p)
			{
				item->next = next->next;
				break;
			}
			item = next;
		}
	}
}

void LeakDetectorClass::exploreNotAllocatedPtr(void* p, const char* file, int line)
{
	AllocatedPtr* item = freedList;
	while (item != nullptr)
	{
		if (item->pointer == p)
		{
			addIssue(p, IssueType::DoubleDeallocation, item->size, file, line, item->file, item->line);
			return;
		}
		item = item->next;
	}

	addIssue(p, IssueType::DoubleDeallocation, 0, file, line);
}

void LeakDetectorClass::addIssue(void* pointer, LeakDetectorClass::IssueType type, size_t size, const char* file, int line, const char* file2, int line2)
{
	void* nextItemAddr = malloc(sizeof(MemoryIssue));
	MemoryIssue* nextItem = new (nextItemAddr) MemoryIssue(pointer, type, size, file, line, file2, line2);

	if (memoryIssuesFirst == nullptr)
	{
		memoryIssuesFirst = nextItem;
	}

	if (memoryIssuesLast != nullptr)
	{
		memoryIssuesLast->next = nextItem;
	}
	memoryIssuesLast = nextItem;
}

template<typename... Args>
static void writeOfstreamLine(std::ofstream& ofstream, Args... args)
{
	if (ofstream.is_open())
	{
		((ofstream << args), ...);
		ofstream << "\n";
	}

	((std::clog << args), ...);
	std::clog << "\n";
}

void LeakDetectorClass::printIssues() noexcept
{
	MemoryIssue* localIssuesList;
	{
		std::lock_guard<std::recursive_mutex> g(mutex);

		AllocatedPtr* item = allocatedList;
		while (item != nullptr)
		{
			addIssue(item->pointer, IssueType::Leak, item->size, item->file, item->line);
			item = item->next;
		}

		// "move" the list to this function to ignore all the allocations happened in this function
		localIssuesList = memoryIssuesFirst;
		memoryIssuesFirst = nullptr;
		memoryIssuesLast = nullptr;

		freeAllocatedList();
		allocatedList = nullptr;

		freeFreedList();
		freedList = nullptr;
	}

	{
		namespace fs = std::filesystem;
		if (!fs::is_directory("./logs") || !fs::exists("./logs"))
		{
			fs::create_directory("logs");
		}

		std::ofstream logFileStream("./logs/leaks_report.txt", std::ios_base::trunc);
		logFileStream << std::showbase;

		writeOfstreamLine(logFileStream, "Memory leak report started");

		if (localIssuesList == nullptr)
		{
			writeOfstreamLine(logFileStream, "No memory issues detected");
		}

		MemoryIssue* item = localIssuesList;
		while (item != nullptr)
		{
			switch (item->type) {
			case IssueType::Leak:
				writeOfstreamLine(logFileStream, "Error: Leaked ", item->size, " bytes (", std::hex, item->pointer, std::dec, ") allocated in ", item->file, "(", item->line, ")");
				break;
			case IssueType::DoubleDeallocation:
				writeOfstreamLine(logFileStream, "Error: Detected a try to deallocate memory twice (", std::hex, item->pointer, std::dec, ") ", item->size, " bytes ", item->file, "(", item->line, ") allocated in ", item->file2, "(", item->line2, ")");
				break;
			case IssueType::DeallocationNotAllocated:
				writeOfstreamLine(logFileStream, "Error: Detected a try to deallocate not allocated pointer (", std::hex, item->pointer, std::dec, ") at", item->file, "(", item->line, ")");
				break;
			}

			item = item->next;
		}
		writeOfstreamLine(logFileStream, "Memory leak report finished");

		freeIssuesList(localIssuesList);
	}
}

void LeakDetectorClass::freeAllocatedList()
{
	AllocatedPtr* item = allocatedList;
	while (item != nullptr)
	{
		AllocatedPtr* next = item->next;
		item->~AllocatedPtr();
		free(item);
		item = next;
	}
}

void LeakDetectorClass::freeFreedList()
{
	AllocatedPtr* item = freedList;
	while (item != nullptr)
	{
		AllocatedPtr* next = item->next;
		item->~AllocatedPtr();
		free(item);
		item = next;
	}
}

void LeakDetectorClass::freeIssuesList(LeakDetectorClass::MemoryIssue* listBegin)
{
	MemoryIssue* item = listBegin;
	while (item != nullptr)
	{
		MemoryIssue* next = item->next;
		item->~MemoryIssue();
		free(item);
		item = next;
	}
}

void* operator new(size_t size, const char* file, int line)
{
	void* p = malloc(size);
	LeakDetectorClass::Get().AddAllocated(p, size, file, line);
	return p;
}

void* operator new[](size_t size, const char* file, int line)
{
	void* p = malloc(size);
	LeakDetectorClass::Get().AddAllocated(p, size, file, line);
	return p;
}

void operator delete(void* ptr, const char* file, int line) noexcept
{
	LeakDetectorClass::Get().AddDeallocated(ptr, file, line);
	free(ptr);
}

void operator delete[](void* ptr, const char* file, int line) noexcept
{
	LeakDetectorClass::Get().AddDeallocated(ptr, file, line);
	free(ptr);
}

void* operator new(size_t size)
{
	return operator new(size, "<Unknown>", 0);
}

void* operator new[](size_t size)
{
	return operator new[](size, "<Unknown>", 0);
}

void operator delete(void* p) noexcept
{
	operator delete(p, "<Unknown>", 0);
}

void operator delete(void* p, size_t) noexcept
{
	operator delete(p, "<Unknown>", 0);
}

void operator delete[](void* p) noexcept
{
	operator delete[](p, "<Unknown>", 0);
}

void operator delete[](void* p, size_t) noexcept
{
	operator delete[](p, "<Unknown>", 0);
}

#endif // DETECT_MEMORY_LEAKS
