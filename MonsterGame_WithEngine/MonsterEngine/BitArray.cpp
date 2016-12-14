#include "BitArray.h"


BitArray * BitArray::Create(const size_t num_of_bits, bool start_cleared, GAllocator * my_allocator)
{


	//don't forget that we need this many size_t elements
	size_t bytes_to_set = (num_of_bits / 8) * sizeof(size_t) + sizeof(BitArray);

	size_t number_of_bytes = (num_of_bits / 8) * sizeof(size_t);

	//allocate memory
	void* heap_memory = my_allocator->GAlloc(bytes_to_set);	

	//the actual bit array is just a bit past the structure itself
	size_t* bits_memory = reinterpret_cast<size_t*>(reinterpret_cast<uint8_t*>(heap_memory) + sizeof(BitArray));

	//create a new bit array using placement new, bypassing the overridden new
	return new (heap_memory) BitArray(num_of_bits, bits_memory,number_of_bytes);
}

BitArray::~BitArray()
{
	//TODO: Check for outstanding allocations
	GAllocator::DestroyInstance();
	
}

void BitArray::ClearAll()
{
	memset(bits_, 0xFF, (number_of_bits_ / 8) * sizeof(size_t));
}

void BitArray::SetAll()
{
	memset(bits_, 0x00, (number_of_bits_ / 8) * sizeof(size_t));
	//walk through the entirety of the bit array setting each individually
}

bool BitArray::AreAllClear()
{
	for (size_t i = 0; i < number_of_bits_; i++) {
		if (!(*(bits_) << i*8) && 0xFF) {
			return false;
		}
	}
	return true;
}

bool BitArray::AreAllSet()
{
	for (size_t i = 0; i < number_of_bits_; i++) {
		if (!(*(bits_) << i * 8) && 0x00) {
			return false;
		}
	}
	return true;
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

//////////
//PRIVATES
//////////
BitArray::BitArray(const size_t num_of_bits, size_t* created_bit_array, size_t num_of_bytes) :
	number_of_bits_(num_of_bits),
	bits_(created_bit_array),
	number_of_bytes(num_of_bytes)
{
	DEBUGLOG("BitArray created with %zu bits ", num_of_bits);
	memset(bits_, 0xFF, (num_of_bits / 8) * sizeof(size_t));
}