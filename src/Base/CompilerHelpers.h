#pragma once

#ifdef __has_cpp_attribute
	#if __has_cpp_attribute(likely)
		#define ALMOST_ALWAYS(expr) [[likely]] (expr)
	#endif
	#if __has_cpp_attribute(unlikely)
		#define ALMOST_NEVER(expr) [[unlikely]] (expr)
	#endif
#endif
#ifndef ALMOST_ALWAYS
	#if defined(__builtin_expect)
		#define ALMOST_ALWAYS(expr) (__builtin_expect(static_cast<bool>(expr), 1))
	#else
		#define ALMOST_ALWAYS(expr) (expr)
	#endif
#endif
#ifndef ALMOST_NEVER
	#if defined(__builtin_expect)
		#define ALMOST_NEVER(expr) (__builtin_expect(static_cast<bool>(expr), 0))
	#else
		#define ALMOST_NEVER(expr) (expr)
	#endif
#endif

#ifdef __has_cpp_attribute
	#if __has_cpp_attribute(maybe_unused)
		#define MAYBE_UNUSED [[maybe_unused]]
	#endif
#endif
#ifndef MAYBE_UNUSED
	#ifdef WIN32
	    #define MAYBE_UNUSED
	#else
	    #define MAYBE_UNUSED __attribute__((unused))
	#endif
#endif
