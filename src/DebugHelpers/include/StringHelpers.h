#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

template<typename ... Args>
std::string FormatString(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
