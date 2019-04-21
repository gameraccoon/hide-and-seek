#pragma once
#include "Log.h"

#ifdef DEBUG
	#define AssertInner(cond, message, ...) \
	do \
	{ \
		if (!(cond)) \
		{ \
			LogError("Assertion failed '%s' %s line %d. Message: " message " (%d)", STR(cond), __FILE__, __LINE__, ##__VA_ARGS__); \
		} \
	} while(0)
#else
	#define AssertInner(cond, message) \
		do { (void)sizeof(cond); (void)sizeof(message); } while(0)
#endif
#define Assert(cond, ...) AssertInner(cond, ##__VA_ARGS__, 0)

#define ReportFailureInner(message, ...) \
do \
{ \
	AssertInner(false, message, ##__VA_ARGS__); \
} while(0)
#define ReportFailure(...) ReportFailureInner(##__VA_ARGS__, 0)

// macros for lazy programmers below (check condition and return/break/etc. even in release)

#define AssertRetInner(cond, ret, message, ...) \
do { \
	if (static_cast<bool>(cond) == false) \
	{ \
		AssertInner(false, message, ##__VA_ARGS__); \
		return (ret); \
	} \
} while(0)
#define AssertRet(cond, ret, ...) AssertRetInner(cond, ret, ##__VA_ARGS__, 0)

#define AssertRetVoidInner(cond, message, ...) \
do { \
	if (static_cast<bool>(cond) == false) \
	{ \
		AssertInner(false, message, ##__VA_ARGS__); \
		return; \
	} \
} while(0)
#define AssertRetVoid(cond, ...) AssertRetVoidInner(cond, ##__VA_ARGS__, 0)

#define AssertBreakInner(cond, message, ...) \
do { \
	if (static_cast<bool>(cond) == false) \
	{ \
		AssertInner(false, message, ##__VA_ARGS__); \
		break; \
	} \
} while(0)
#define AssertBreak(cond, ...) AssertBreakInner(cond, ##__VA_ARGS__, 0)

#define AssertContinueInner(cond, message, ...) \
do { \
	if (static_cast<bool>(cond) == false) \
	{ \
		AssertInner(false, message, ##__VA_ARGS__); \
		continue; \
	} \
} while(0)
#define AssertContinue(cond, ...) AssertContinueInner(cond, ##__VA_ARGS__, 0)
