#pragma once

#include <inttypes.h>

// don't use theese in your code
using raw_i32 = int32_t;
using raw_u32 = int32_t;
using raw_i64 = int64_t;
using raw_u64 = uint64_t;

// can be aliased to some other types in debug mode for overflows checking and other debug stuff
using i32 = raw_i32;
using u32 = raw_u32;
using i64 = raw_i64;
using u64 = raw_u64;
