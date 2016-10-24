#ifndef MONSTERDEBUG_H
#define MONSTERDEBUG_H

#include <stdarg.h>
#include <stdio.h>



class MonsterDebug
{
public: 
	static void DebugPrint(const char * input, ...);



};

#ifdef _DEBUG
#define DEBUGLOG(fmt,...) MonsterDebug::DebugPrint(fmt,__VA_ARGS__)
#define DEBUG_LIST_DISPLAY MonsterAllocator::PrintLists()
#else
#define DEBUGLOG(fmt,...) void(0)
#define DEBUG_LIST_DISPLAY void (0)
#endif

#endif
