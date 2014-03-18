
// Standart structures e.g. NULL
#include <stdio.h>
#include <iostream>

// Debugging methods
#if defined(DEBUG)
	#define WARN(message)					\
	{										\
		std::cout << "ERROR: " << message;	\
		std::cout << std::endl;				\
		exit(1);							\
	}

	#define WARN_IF(condition, message)		\
	{										\
		if (condition)						\
		{									\
			std::cout << "On assertion ";	\
			std::cout << condition;			\
			std::cout << std::endl;			\
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