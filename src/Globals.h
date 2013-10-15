#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H
// Global things, that must be available in any game

// Include all needed HGE classes
#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgeparticle.h>

// Standart structures e.g. NULL
#include <stdio.h>
// More structures e.g. counnt and endl 
#include <iostream>

// comment this definition in release
#define DEBUG

// Debugging methods
#if defined(DEBUG)
	#define WARN(message)					\
	{										\
		std::cout << "Warning: ";			\
		std::cout << message << std::endl;	\
	}

	#define WARN_IF(condition, message)	\
	{									\
		if (condition)					\
		{								\
			WARN(message);				\
		}								\
	}
#endif

/*
 * In the headers of classes that will use macroses
 * WARN or WARN_IF it's necessary to include next
 * instructions
 */
  //// dummy for debugging methods
  //#if (!defined DEBUG) && (!defined RELEASE)
 	//#define RELEASE
  //	#define WARN(message)
  //	#define WARN_IF(condition, message)
  //#endif
/*
 * It will provide reusability of these classes in
 * other programs.
 */

/** HGE subsystem */
extern HGE *Hge;

#endif