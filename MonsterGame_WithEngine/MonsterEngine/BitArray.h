#pragma once

#include "MonsterDebug.h"
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

	inline bool IsSet(size_t bit_number) const;
	inline bool IsClear(size_t bit_number) const;

	void SetBit(const size_t bit_to_set);
	void ClearBit(const size_t bit_to_clear);

private:
	size_t number_of_bits_;
	size_t* bits_;

	bool IsValidBlockAddr(const void * addr_to_check) const;
};

