#pragma once

#include <inttypes.h>

// don't use theese in your code
using raw_s32 = int32_t;
using raw_u32 = int32_t;
using raw_s64 = int64_t;
using raw_u64 = uint64_t;
using raw_f32 = float;
using raw_f64 = double;

// can be aliased to some other types in debug mode for overflows checking and other debug stuff
using s32 = raw_s32;
using u32 = raw_u32;
using s64 = raw_s64;
using u64 = raw_u64;
using f32 = raw_f32;
using f64 = raw_f64;
