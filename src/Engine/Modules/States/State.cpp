#include "State.h"

#include "../../Helpers/DebugMethods.h"

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
 	#define RELEASE
  	#define WARN(message)
  	#define WARN_IF(condition, message)
#endif

void State::process()
{
    WARN("Function 'process' not redefined in some state");
}

State::~State()
{
}
