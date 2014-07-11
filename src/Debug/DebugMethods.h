
#include "Log.h"

// Debugging methods
#if defined(DEBUG) && (!defined RELEASE)
	#define WARN(message)						\
	{											\
		Log::Instance().writeError(std::string(message));	\
		exit(1);								\
	}

	#define WARN_IF(condition, message)		\
	{										\
		if (condition)						\
		{									\
			WARN(message);					\
		}									\
	}
#endif

/*
 * In the headers of classes that will use macroses
 * WARN or WARN_IF it's necessary to include next
 * instructions
 */
// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
 	#define RELEASE
  	#define WARN(message)
  	#define WARN_IF(condition, message)
#endif
/*
 * It will provide reusability of these classes in
 * other programs.
 */