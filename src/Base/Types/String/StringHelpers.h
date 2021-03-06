#pragma once

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

inline const char* FormatSprinfableArgument(const std::string& arg)
{
	return arg.c_str();
}

inline const char* FormatSprinfableArgument(std::string&& arg)
{
	return arg.c_str();
}

inline const char* FormatSprinfableArgument(StringID arg)
{
	static_assert(std::is_same_v<std::invoke_result_t<decltype(&StringIDManager::getStringFromID), StringIDManager, StringID>, const std::string&>, "StringIDManager::getStringFromID should return const ref to std::string");
	return StringIDManager::Instance().getStringFromID(arg).c_str();
}

template<typename T>
auto FormatSprinfableArgument(T arg)
{
	return std::forward<T>(arg);
}

template<typename... Args>
std::string FormatString(const std::string& format)
{
	return format;
}

template<typename First, typename... Args>
std::string FormatString(const std::string& format, First&& first, Args&&... args)
{
	char buf[4096];
	std::snprintf(buf, sizeof(buf), format.c_str(), FormatSprinfableArgument(std::forward<First>(first)), FormatSprinfableArgument(std::forward<Args>(args))...);
	return std::string(buf);
}
