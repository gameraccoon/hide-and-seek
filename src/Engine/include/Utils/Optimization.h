#pragma once

// define empty for your compiler if it doesn't support it
#define LIKELY(x) __builtin_expect (!!(x), 1)
#define UNLIKELY(x)  __builtin_expect (!!(x), 0)
