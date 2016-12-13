#include "BitArray.h"



BitArray::BitArray(const size_t num_of_bits) :
	number_of_bits_(num_of_bits)
{
	DEBUGLOG("Creating Bitarray");
	DEBUGLOG("Size of size_t: %d", sizeof(size_t));

	bits_ = new size_t[num_of_bits / 8];

	memset(bits_, 0xFF, sizeof(bits_)*4);


}


BitArray::~BitArray()
{
	//TODO: Check for outstanding allocations
	GAllocator::DestroyInstance();
	
}

void BitArray::ClearAll()
{
}

void BitArray::SetAll()
{
}

bool BitArray::AreAllClear()
{
	return false;
}

bool BitArray::AreAllSet()
{
	return false;
}

inline bool BitArray::IsSet(size_t bit_number) const
{
	size_t val = (*(bits_) >> bit_number) & 1;
	if (val == 1)
		return true;
	else
		return false;
	
}

inline bool BitArray::IsClear(size_t bit_number) const
{
	return false;
}

void BitArray::SetBit(const size_t bit_to_set)
{

	*(bits_) |= 1 << bit_to_set;
}

void BitArray::ClearBit(const size_t bit_to_clear)
{
	*(bits_) &= ~(1 << bit_to_clear);
}

bool BitArray::IsValidBlockAddr(const void * addr_to_check) const
{
	return false;
}
