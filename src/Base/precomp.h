#include <cstdio>
#include <fstream>
#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>

#include <nlohmann/json_fwd.hpp>
#include <fnv1a/hash_fnv1a_constexpr.h>

#include "Base/CompilerHelpers.h"
#include "Base/Debug/MemoryLeakDetection.h"
#include "Base/Types/String/StringID.h"
#include "Base/Types/String/StringHelpers.h"
#include "Base/Debug/Log.h"
#include "Base/Debug/Assert.h"
