#include "BitArray.h"



BitArray::BitArray(const size_t num_of_bits) :
	number_of_bits_(num_of_bits)
{
	DEBUGLOG("Creating Bitarray");

	bits_ = new size_t[num_of_bits / 8];
	memset(bits_, 0xFF, (num_of_bits / 8) * sizeof(size_t));


}


BitArray::~BitArray()
{
	//TODO: Check for outstanding allocations
	GAllocator::DestroyInstance();
	
}

void BitArray::ClearAll()
{
	memset(bits_, 0xFF, sizeof(bits_));
}

void BitArray::SetAll()
{
	memset(bits_, 0x00, sizeof(bits_));
}

bool BitArray::AreAllClear()
{
	return false;
}

bool BitArray::AreAllSet()
{
	return false;
}

bool BitArray::IsSet(size_t bit_number) const
{
	size_t val = (*(bits_) >> bit_number*4) & 1;
	if (val == 1)
		return true;
	else
		return false;
	
}

bool BitArray::IsClear(size_t bit_number) const
{
	size_t val = (*(bits_) >> bit_number*4) & 1;
	if (val == 0)
		return true;
	else
		return false;
}

void BitArray::SetBit(const size_t bit_to_set)
{
	*(bits_) |= 0xF << bit_to_set*4;
}

void BitArray::ClearBit(const size_t bit_to_clear)
{
	*(bits_) &= ~(0xF << bit_to_clear * 4);
}

bool BitArray::GetFirstClearBit(size_t & o_index) const
{
	return false;
}

bool BitArray::GetFirstSetBit(size_t & o_index) const
{
	return false;
}

inline bool BitArray::operator[](const size_t index)
{
	return IsSet(index);
}

