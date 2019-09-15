#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <cstdio>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

template<typename... Args>
std::string FormatString(const std::string& format)
{
	return format;
}

template<typename First, typename... Args>
std::string FormatString(const std::string& format, First first, Args... args)
{
	char buf[4096];
	snprintf(buf, sizeof(buf), format.c_str(), std::forward<First>(first), std::forward<Args>(args)...);
	return std::string(buf);
}
