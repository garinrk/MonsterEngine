#pragma once

#include "BitArray.h"
#include "FixedSizeAllocator.h"
#include "GAllocator.h"
#include "MemoryManager.h"
#include "MonsterDebug.h"

#include <algorithm>
#include <assert.h>
#include <vector>
class MonsterTesting {
public:
	static bool GAllocatorTests();
	static bool GAllocatorWithAlignmentTests();
	static bool BitArrayTests();
	static bool FSATests();
	static bool FinalHeapManagerTesting();
};
