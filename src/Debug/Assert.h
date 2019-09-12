#pragma once

#include "Log.h"
#include "OptimizationHelpers.h"

#ifdef DEBUG
#define ReportError(...) \
	do \
	{ \
		LogAssertHelper("false", __FILE__, __LINE__, ##__VA_ARGS__); \
	} while(0)
#else
#define ReportError(...) do { } while(0)
#endif

#ifdef DEBUG
#define ReportFatalError(...) \
	do \
	{ \
		LogAssertHelper("false", __FILE__, __LINE__, ##__VA_ARGS__); \
		std::terminate(); \
	} while(0)
#else
#define ReportFatalError(...) do { } while(0)
#endif

#ifdef DEBUG
	#define Assert(cond, ...) \
	do \
	{ \
		if almost_never(static_cast<bool>(cond) == false) \
		{ \
			LogAssertHelper(STR(cond), __FILE__, __LINE__, ##__VA_ARGS__); \
		} \
	} while(0)
#else
	#define Assert(...) do { } while(0)
#endif

#ifdef DEBUG
#define AssertFatal(cond, ...) \
do { \
	if almost_never(static_cast<bool>(cond) == false) \
	{ \
		LogAssertHelper(STR(cond), __FILE__, __LINE__, ##__VA_ARGS__); \
		std::terminate(); \
	} \
} while(0)
#else
#define AssertFatal(...) do { } while(0)
#endif

// macros for lazy programmers below (check condition and return/break/etc. even in release)

#define AssertRet(cond, ret, ...) \
do { \
	if almost_never(static_cast<bool>(cond) == false) \
	{ \
		LogAssertHelper(STR(cond), __FILE__, __LINE__, ##__VA_ARGS__); \
		return (ret); \
	} \
} while(0)

#define AssertRetVoid(cond, ...) \
do { \
	if almost_never(static_cast<bool>(cond) == false) \
	{ \
		LogAssertHelper(STR(cond), __FILE__, __LINE__, ##__VA_ARGS__); \
		return; \
	} \
} while(0)

#define AssertBreak(cond, ...) \
do { \
	if almost_never(static_cast<bool>(cond) == false) \
	{ \
		LogAssertHelper(STR(cond), __FILE__, __LINE__, ##__VA_ARGS__); \
		break; \
	} \
} while(0)

#define AssertContinue(cond, ...) \
do { \
	if almost_never(static_cast<bool>(cond) == false) \
	{ \
		LogAssertHelper(STR(cond), __FILE__, __LINE__, ##__VA_ARGS__); \
		continue; \
	} \
} while(0)
