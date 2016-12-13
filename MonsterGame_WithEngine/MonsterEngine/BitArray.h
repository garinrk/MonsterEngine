#pragma once

#include "MonsterDebug.h"
#include "GAllocator.h"
#include <string.h>
class BitArray
{
public:
	BitArray(const size_t num_of_bits);
	~BitArray();

	void ClearAll();
	void SetAll();

	bool AreAllClear();
	bool AreAllSet();

	bool IsSet(size_t bit_number) const;
	bool IsClear(size_t bit_number) const;

	void SetBit(const size_t bit_to_set);
	void ClearBit(const size_t bit_to_clear);
	
	bool GetFirstClearBit(size_t & o_index) const;
	bool GetFirstSetBit(size_t & o_index) const;

	inline bool operator[](const size_t index);
private:
	size_t number_of_bits_;
	size_t* bits_;

};

