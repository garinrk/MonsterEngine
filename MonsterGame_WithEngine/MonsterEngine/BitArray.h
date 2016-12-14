#pragma once

#include "MonsterDebug.h"
#include "GAllocator.h"
#include <string.h>
#include <new>
class BitArray
{
public:
	static BitArray * Create(const size_t num_of_bits, bool start_cleared, GAllocator * my_allocator);
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
	size_t number_of_bytes;

	BitArray(const size_t num_of_bits, size_t* created_bit_array, size_t num_of_bytes);

	const size_t bits_per_byte = 8;
};

