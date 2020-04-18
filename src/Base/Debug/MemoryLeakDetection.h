#pragma once

#ifdef DETECT_MEMORY_LEAKS

#include <mutex>
#include <filesystem>
#include <fstream>
#include <iostream>

class LeakDetectorClass
{
public:
	static LeakDetectorClass& Get()
	{
		static LeakDetectorClass instance;
		return instance;
	}

	~LeakDetectorClass();

	void AddAllocated(void* p, size_t size, const char* file, int line) noexcept;
	void AddDeallocated(void* p, const char* file, int line) noexcept;

private:
	// single-linked list node
    struct AllocatedPtr
    {
        AllocatedPtr(void* pointer, size_t size, const char* file, int line, AllocatedPtr* next)
			: pointer(pointer), next(next), size(size), file(file), line(line) {}

        void* pointer;
		AllocatedPtr* next;
        size_t size;
        const char* file;
        int line;
    };

	enum class IssueType
	{
		Leak,
		DoubleDeallocation,
		DeallocationNotAllocated
	};

	// double-linked list node
	struct MemoryIssue
	{
		MemoryIssue(void* pointer, IssueType type, size_t size, const char* file, int line, const char* file2, int line2)
			: pointer(pointer), type(type), size(size), file(file), line(line), file2(file2), line2(line2) {}

		MemoryIssue* next = nullptr;
		void* pointer;
		IssueType type;
		size_t size;
		const char* file;
		int line;
		const char* file2;
		int line2;
	};

	void freeAllocatedItem(AllocatedPtr* item);
	void clearFreedPointer(void* p);

	void exploreNotAllocatedPtr(void* p, const char* file, int line);

	void addIssue(void* pointer, IssueType type, size_t size, const char* file, int line, const char* file2 = nullptr, int line2 = 0);
	void printIssues() noexcept;

	void freeAllocatedList();
	void freeFreedList();
	void freeIssuesList(MemoryIssue* listBegin);

	AllocatedPtr* allocatedList = nullptr;
	AllocatedPtr* freedList = nullptr;

	MemoryIssue* memoryIssuesFirst = nullptr;
	MemoryIssue* memoryIssuesLast = nullptr;

	std::recursive_mutex mutex;
};

void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);
void operator delete(void* ptr, const char* file, int line) noexcept;
void operator delete[](void* ptr, const char* file, int line) noexcept;

void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void* p) noexcept;
void operator delete(void* p, size_t /*size*/) noexcept;

void operator delete[](void* p) noexcept;
void operator delete[](void* p, size_t /*size*/) noexcept;

#define DEBUG_NEW new(__FILE__, __LINE__)

#ifdef REDEFINE_NEW
#define new DEBUG_NEW
#endif

#define DEBUG_DELETE(p) operator delete(p, __FILE__, __LINE__)

#endif // DETECT_MEMORY_LEAKS
