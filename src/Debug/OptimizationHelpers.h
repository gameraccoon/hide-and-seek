#pragma once

#if defined (__builtin_expect)
	#define almost_always(expr) (__builtin_expect(static_cast<bool>(expr), 1))
	#define almost_never(expr) (__builtin_expect(static_cast<bool>(expr), 0))
#else
	#define almost_always(expr) (expr)
	#define almost_never(expr) (expr)
#endif
