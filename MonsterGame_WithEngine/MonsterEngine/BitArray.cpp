#include "BitArray.h"

BitArray * BitArray::Create(const size_t num_of_bits, bool start_cleared, GAllocator * my_allocator)
{
	//Don't forget!
	const size_t bits_per_byte = 8;
	
	//given the amount of bits needed, how many containers will we be needing? 32 - 64 dependent
	const size_t amt_per_container = bits_per_byte * sizeof(bitContainer);

	//now, how many containers are we gonna need to store the amount of bits that you need?
	size_t number_of_containers = (num_of_bits + (amt_per_container - 1)) / amt_per_container;

	//how much memory we gonna need for all these containers and the bit array structure itself?
	size_t total_bytes_needed = number_of_containers * sizeof(bitContainer) + sizeof(BitArray);

	//let's get dat memory
	void * memory_chunk = my_allocator->GAlloc(total_bytes_needed);

	//but wait, where are the actual bits gonna be? Past the BitArray!
	size_t* bits_memory = reinterpret_cast<size_t*>(reinterpret_cast<uintptr_t>(memory_chunk) + sizeof(BitArray));

	//create dat bit array.
	return new (memory_chunk) BitArray(num_of_bits, total_bytes_needed, number_of_containers, bits_memory);
}

BitArray::~BitArray()
{
	//TODO: Check for outstanding allocations
	GAllocator::DestroyInstance();
	
}

void BitArray::ClearAll()
{
	memset(bits_, 0x00, number_of_bytes - sizeof(BitArray));
	//memset(bits_, 0x00, (number_of_bits_ / 8)/* * sizeof(size_t)*/);
}

void BitArray::SetAll()
{
	memset(bits_, 0xFF, number_of_bytes - sizeof(BitArray));
}

bool BitArray::AreAllClear()
{
	//using bitscanforward, look through all the containers
	for (size_t i = 0; i < number_of_containers; i++) {
		unsigned long index = 0;
		//position of container
		if (BITSCAN(&index, bits_[i])) {
			return false;
		}

		
	}

	return true;
}

bool BitArray::AreAllSet()
{
	//TODO: TRIPLE CHECK
	//using bitscanforward, look through all the containers
	for (size_t i = 0; i < number_of_containers; i++) {
		unsigned long index = 0;
		//position of container
		if (!BITSCAN(&index, bits_[i])) {
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
	*(bits_) |= 1 << bit_to_set;
}

void BitArray::ClearBit(const size_t bit_to_clear)
{
	*(bits_) &= ~(1 << bit_to_clear);
}

bool BitArray::GetFirstClearBit(size_t & o_index) const
{
	//negate and use the inverse to find the first set
	for (size_t i = 0; i < number_of_containers; i++) {
		unsigned long index = 0;

		size_t negated_bits = ~(*bits_+ sizeof(bitContainer) * i);

		//position of container
		if (BITSCAN(&index, negated_bits)) {
			o_index = static_cast<size_t>(index);
			return true;
		}
	}
	return false;
}

bool BitArray::GetFirstSetBit(size_t & o_index) const
{
	//using bitscanforward, look through all the containers
	for (size_t i = 0; i < number_of_containers; i++) {
		unsigned long index = 0;
		//position of container
		if (BITSCAN(&index, bits_[i])) {
			o_index = static_cast<size_t>(index);
			return true;
		}
	}

	return false;
}

inline bool BitArray::operator[](const size_t index)
{
	return IsSet(index);
}

//////////
//PRIVATES
//////////
BitArray::BitArray(const size_t amt_of_user_requested_bits, size_t amt_of_bytes, size_t amt_of_containers, size_t* bits_array_addr) :
	number_of_bits_(amt_of_user_requested_bits),
	number_of_bytes(amt_of_bytes),
	number_of_containers(amt_of_containers),
	bits_(bits_array_addr)
	
{
	DEBUGLOG("BitArray created for %zu bits\t%zu containers\t%zu bytes ", amt_of_user_requested_bits,amt_of_containers, amt_of_bytes);

	//lets set all da containers to empty.
	memset(bits_, 0xFF, amt_of_bytes - sizeof(BitArray));

	//memset(bits_, 0xFF, (num_of_user_requested_bits / 8) /** sizeof(bitContainer)*/);
}