#include "Base/precomp.h"

#include "Base/Debug/MemoryLeakDetection.h"

#ifdef DETECT_MEMORY_LEAKS

LeakDetectorClass::~LeakDetectorClass()
{
	printLeaks();

	freeAllocatedList();
	freeLeaksList(memoryLeaksFirst);
}

void LeakDetectorClass::AddAllocated(void* p, size_t size, const char* file, int line) noexcept
{
	std::lock_guard<std::recursive_mutex> g(mutex);

	void* nextItemAddr = malloc(sizeof(AllocatedPtr));
	AllocatedPtr* nextItem = new (nextItemAddr) AllocatedPtr(p, size, file, line, allocatedList);
	allocatedList = nextItem;
}

void LeakDetectorClass::RemoveAllocated(void* p) noexcept
{
	std::lock_guard<std::recursive_mutex> g(mutex);

	if (p == nullptr)
	{
		return;
	}

	if (allocatedList == nullptr)
	{
		return;
	}

	// first item needs a special treatment, since allocatedList points to it
	if (allocatedList->pointer == p)
	{
		AllocatedPtr* item = allocatedList;
		allocatedList = allocatedList->next;
		free(item);
		return;
	}

	AllocatedPtr* item = allocatedList;

	while (item->next != nullptr)
	{
		AllocatedPtr* next = item->next;
		if (next->pointer == p)
		{
			item->next = next->next;
			free(next);
			return;
		}
		item = next;
	}
}

void LeakDetectorClass::addLeakInfo(void* pointer, size_t size, const char* file, int line)
{
	void* nextItemAddr = malloc(sizeof(MemoryLeak));
	MemoryLeak* nextItem = new (nextItemAddr) MemoryLeak(pointer, size, file, line);

	if (memoryLeaksFirst == nullptr)
	{
		memoryLeaksFirst = nextItem;
	}

	if (memoryLeaksLast != nullptr)
	{
		memoryLeaksLast->next = nextItem;
	}
	memoryLeaksLast = nextItem;
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

void LeakDetectorClass::printLeaks() noexcept
{
	MemoryLeak* localLeaksList;
	{
		std::lock_guard<std::recursive_mutex> g(mutex);

		AllocatedPtr* item = allocatedList;
		while (item != nullptr)
		{
			addLeakInfo(item->pointer, item->size, item->file, item->line);
			item = item->next;
		}

		// "move" the list to this function to ignore all the allocations happened in this function
		localLeaksList = memoryLeaksFirst;
		memoryLeaksFirst = nullptr;
		memoryLeaksLast = nullptr;

		freeAllocatedList();
		allocatedList = nullptr;
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

		if (localLeaksList == nullptr)
		{
			writeOfstreamLine(logFileStream, "No memory leaks detected");
		}

		MemoryLeak* item = localLeaksList;
		size_t leakedBytes = 0;
		while (item != nullptr)
		{
			writeOfstreamLine(logFileStream, "Error: Leaked ", item->size, " bytes (", std::hex, item->pointer, std::dec, ") allocated in ", item->file, "(", item->line, ")");
			leakedBytes += item->size;
			item = item->next;
		}

		if (leakedBytes > 0)
		{
			writeOfstreamLine(logFileStream, "Total leaked bytes: ", leakedBytes);
		}

		writeOfstreamLine(logFileStream, "Memory leak report finished");

		freeLeaksList(localLeaksList);
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

void LeakDetectorClass::freeLeaksList(LeakDetectorClass::MemoryLeak* listBegin)
{
	MemoryLeak* item = listBegin;
	while (item != nullptr)
	{
		MemoryLeak* next = item->next;
		item->~MemoryLeak();
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
	LeakDetectorClass::Get().RemoveAllocated(p);
	free(p);
}

void operator delete(void* p, size_t) noexcept
{
	LeakDetectorClass::Get().RemoveAllocated(p);
	free(p);
}

void operator delete[](void* p) noexcept
{
	LeakDetectorClass::Get().RemoveAllocated(p);
	free(p);
}

void operator delete[](void* p, size_t) noexcept
{
	LeakDetectorClass::Get().RemoveAllocated(p);
	free(p);
}

#endif // DETECT_MEMORY_LEAKS
