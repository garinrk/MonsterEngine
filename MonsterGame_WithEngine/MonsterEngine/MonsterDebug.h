#ifndef MONSTERDEBUG_H
#define MONSTERDEBUG_H

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>


class MonsterDebug
{
public:
	static void DebugPrint(const char * i_format, ...);



};

#ifdef _DEBUG
#define DEBUGLOG(fmt,...) MonsterDebug::DebugPrint(fmt,__VA_ARGS__)
//#define PRINT_GALLOC_STATE GAllocator::PrintAllocatorState()
#define PRINT_GALLOC_STATE void(0)
//#define DEBUGLOG(fmt,...) void(0)
#else
#define DEBUGLOG(fmt,...) void(0)
#define DEBUG_LIST_DISPLAY void (0)
#define PRINT_LIST(root) void(0)
#define PRINT_GALLOC_STATE void(0)
#endif

#endif
