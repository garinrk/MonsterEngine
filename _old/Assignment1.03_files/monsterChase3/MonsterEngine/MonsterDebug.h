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
#else
#define DEBUGLOG(fmt,...) void(0)
#endif

#endif
