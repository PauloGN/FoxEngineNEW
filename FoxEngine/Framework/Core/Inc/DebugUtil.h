#pragma once

#include "TimeUtil.h"

#if defined(_DEBUG)
#define LOG(format, ...)\
	do{\
		char _buffer[256];\
		int _res = snprintf(_buffer, std::size(_buffer), "[%.3f]: "##format##"\n", FoxEngine::Core::TimeUtil::GetTime(), __VA_ARGS__);\
		OutputDebugStringA(_buffer);\
	} while (false)

#define ASSERT(condition, format, ...)\
	do{\
		if(!(condition))\
		{\
			LOG("ASSERT! %s(%d)\n"##format, __FILE__, __LINE__, __VA_ARGS__);\
			DebugBreak();\
		}\
	} while (false)
#else
#define LOG(format, ...)
#define ASSERT(condition, format, ...) do { (void)(sizeof(condition));}while(false)
#endif // DEBUG

/*
 * do{}while reason
 *
 *  if(true){ do this} ***
 *	do{	if(conditions){do macro stuff}} while(false)
 *	else{ do that } ***
 *
 *  as macro is basically copy and past with out the 'do while' it would become the following
 *
 *	if(true){do this}
 *		if(conditions){do macro stuff} ***
 *		else{ do that } ***
 *
 *		it would change the meaning of the original code
 */