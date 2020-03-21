#pragma once

// use these preprocessor definitions to enable leak detection features
// DETECT_MEMORY_LEAKS, DETECT_DOUBLE_DELETE

#ifdef DETECT_MEMORY_LEAKS

#include <mutex>
#include <filesystem>
#include <fstream>
#include <iostream>

class LeakDetectorClass
{
public:
    LeakDetectorClass()
    {
        const std::string LOG_FILE = std::string("./logs/").append("leaks_report.txt");

        namespace fs = std::filesystem;
        if (!fs::is_directory("./logs") || !fs::exists("./logs"))
        {
            fs::create_directory("logs");
        }

        mLogFileStream = std::ofstream(LOG_FILE, std::ios_base::trunc);
        writeLine("Memory leak report file created");
    }

    template<typename... Args>
    void writeLine(Args... args)
    {
        if (mLogFileStream.is_open())
        {
            ((mLogFileStream << args), ...);
            mLogFileStream << "\n";
        }

        ((std::clog << args), ...);
        std::clog << "\n";
    }

    ~LeakDetectorClass()
    {
        PrintLeaked();

        AllocatedPtr* item = allocatedList;
        while (item != nullptr)
        {
            AllocatedPtr* next = item->next;
            item->~AllocatedPtr();
            free(item);
            item = next;
        }

        item = freedList;
        while (item != nullptr)
        {
            AllocatedPtr* next = item->next;
            item->~AllocatedPtr();
            free(item);
            item = next;
        }
    }

    void PrintLeaked() noexcept
    {
        std::lock_guard<std::mutex> g(mutex);

        AllocatedPtr* item = allocatedList;
        while (item != nullptr)
        {
            writeLine("leaked ", item->size, " bytes allocated in ", item->file, "(", item->line, ")");
            item = item->next;
        }
    }

    void AddAllocated(void* p, size_t size, const char* file, int line) noexcept
    {
        std::lock_guard<std::mutex> g(mutex);

        void* nextItemAddr = malloc(sizeof(AllocatedPtr));
        AllocatedPtr* nextItem = new (nextItemAddr) AllocatedPtr(p, size, file, line, allocatedList);
        allocatedList = nextItem;

        if constexpr (detectDoubleDeletion)
        {
            clearFreedPointer(p);
        }
    }

    void AddDeallocated(void* p, const char* file, int line) noexcept
    {
        std::lock_guard<std::mutex> g(mutex);

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

private:
    struct AllocatedPtr
    {
        AllocatedPtr(void* pointer, size_t size, const char* file, int line, AllocatedPtr* next)
            : pointer(pointer), size(size), file(file), line(line), next(next) {}

        void* pointer;
        size_t size;
        const char* file;
        int line;
        AllocatedPtr* next;
    };

    void freeAllocatedItem(AllocatedPtr* item)
    {
        if constexpr (detectDoubleDeletion)
        {
            item->next = freedList;
            freedList = item;
        }
        else
        {
            item->~AllocatedPtr();
            free(item);
        }
    }

    void exploreNotAllocatedPtr(void* p, const char* file, int line)
    {
        if constexpr (detectDoubleDeletion)
        {
            AllocatedPtr* item = freedList;
            while (item != nullptr)
            {
                if (item->pointer == p)
                {
                    writeLine("Try to deallocate memory twice ", item->size, " bytes ", file, "(", line, ") allocated in ", item->file, "(", item->line, ")");
                    return;
                }
                item = item->next;
            }
            writeLine("Try to deallocate not allocated pointer at", file, "(", line, ")");
        }
        else
        {
            writeLine("Try to deallocate not allocated pointer or deallocate memory twice at ", file, "(", line, ")");
        }
    }

    void clearFreedPointer(void* p)
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

    AllocatedPtr* allocatedList = nullptr;
    AllocatedPtr* freedList = nullptr;

    std::mutex mutex;

    std::ofstream mLogFileStream;

    constexpr static bool detectDoubleDeletion =
#ifdef DETECT_DOUBLE_DELETE
        true;
#else
        false;
#endif // DETECT_DOUBLE_DELETE

} inline LeakDetector;

inline void* operator new(size_t size, const char* file, int line)
{
    void* p = malloc(size);
    LeakDetector.AddAllocated(p, size, file, line);
    return p;
}

inline void* operator new[](size_t size, const char* file, int line)
{
    void* p = malloc(size);
    LeakDetector.AddAllocated(p, size, file, line);
    return p;
}

inline void operator delete(void* ptr, const char* file, int line)
{
    LeakDetector.AddDeallocated(ptr, file, line);
    free(ptr);
}

inline void operator delete[](void* ptr, const char* file, int line)
{
    LeakDetector.AddDeallocated(ptr, file, line);
    free(ptr);
}

inline void* operator new(size_t size)
{
    return operator new(size, "<Unknown>", 0);
}

inline void* operator new[](size_t size)
{
    return operator new[](size, "<Unknown>", 0);
}

inline void operator delete(void* p)
{
    operator delete(p, "<Unknown>", 0);
}

inline void operator delete[](void* p)
{
    operator delete[](p, "<Unknown>", 0);
}

#define DEBUG_NEW new(__FILE__, __LINE__)
#define new DEBUG_NEW

//#define DEBUG_DELETE delete(__FILE__, __LINE__)
//#define delete DEBUG_DELETE

#endif // DETECT_MEMORY_LEAKS
