#pragma once
#include "MonsterTesting.h"
#include "GAllocator.h"
#include "FixedSizeAllocator.h"
#include "BitArray.h"
#include "MonsterDebug.h"
#include "MemoryManager.h"

#include <assert.h>
#include <algorithm>
#include <vector>
class MonsterTesting {
public:
	static bool GAllocatorTests();
	static bool GAllocatorWithAlignmentTests();
	static bool BitArrayTests();
	static bool FSATests();
	static bool MemoryManagerTests();
};
