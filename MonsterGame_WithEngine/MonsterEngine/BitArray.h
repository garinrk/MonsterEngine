#pragma once

#include "MonsterDebug.h"
#include "GAllocator.h"
#include <string.h>
#include <new>
#ifdef _WIN32
typedef uint32_t	bitContainer;
#else 
typedef uint64_t	bitContainer;
#endif

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
	size_t number_of_bytes;
	size_t number_of_containers;
	size_t* bits_;

	BitArray(const size_t amt_of_user_requested_bits, size_t amt_of_bytes,  size_t amt_of_containers, size_t* bits_array_addr);

	const size_t bits_per_byte = 8;
};

