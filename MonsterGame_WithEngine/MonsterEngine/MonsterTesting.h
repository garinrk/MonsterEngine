#pragma once
#include "MonsterTesting.h"
#include "MonsterAllocator.h"
#include "GAllocator.h"
#include "BitArray.h"
#include "MonsterDebug.h"
#include <assert.h>
#include <algorithm>
#include <vector>
class MonsterTesting {
public:
	static bool ModifiedAllocatorTests();
	static bool OriginalAllocatorTests();
	static bool GAllocatorTests();
	static bool GAllocatorWithAlignmentTests();
	static bool BitArrayTests();
};
